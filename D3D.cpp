#include "d3d11.h"
#include "dxgi.h"
#include "DirectXMath.h"
#include "D3D.h"
#include "Render/Inc/TesselationShaderProgram.h"
#include "Render/Inc/DefaultMaterial.h"
#include "Render/Inc/TesselationDefaultLitMaterial.h"
#include "Skybox.h"
#include "Game/Inc/FirstPlayerController.h"
#include "ObjLoader.h"

using namespace DirectX;

ID3D11Device* dev;
ID3D11DeviceContext* devcon;
IDXGISwapChain* swapchain;
ID3D11RenderTargetView *backbuffer;

ID3D11Buffer* pVBuffer;
ID3D11Buffer* pIBuffer;

ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;
ID3D11DepthStencilState* pDSState;

ID3D11RasterizerState* rasterizerState;

Camera* camera;
FirstPlayerController* player;
//DefaultMaterial* material;
TesselationDefaultLitMaterial* material;
XMFLOAT4X4 transform{};

DirectionLight* dirLight;
PointLight* pointLight;

Skybox* skybox;

float angle = 0.0f;

void D3D_SetViewport(int width, int height);
void InitPipeline();
void InitVertexBuffer();

void D3D_Init(HWND hwnd){
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 4;
    swapChainDesc.Windowed = true;

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
                                  NULL, 0, D3D11_SDK_VERSION,
                                  &swapChainDesc, &swapchain, &dev,
                                  NULL, &devcon);

    ID3D11Texture2D* pBackBuffer;

    RECT rect;
    GetClientRect(hwnd, (LPRECT)&rect);
    D3D_SetViewport(rect.right, rect.bottom);

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = rect.right;
    depthStencilDesc.Height = rect.bottom;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 4;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    dev->CreateDepthStencilState(&dsDesc, &pDSState);

    dev->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
    dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_FRONT;
    rd.DepthBias = 0;
    rd.DepthBiasClamp = 0.0f;
    rd.DepthClipEnable = TRUE;
    rd.MultisampleEnable = true;
    dev->CreateRasterizerState(&rd, &rasterizerState);
    devcon->RSSetState(rasterizerState);

    camera = new Camera(rect.right, rect.bottom, 59.0f, 0.1f, 300.0f);
    camera->SetTransform(XMFLOAT3(0.0f, 0.0f, -0.55f), XMFLOAT3(0.0f, 0.0f, 1.0f));
//    ShaderProgram* defaultShader = new ShaderProgram(dev, devcon, L"shader1.hlsl",
//                                                     "VShader", "PShader");
    TesselationShaderProgram* defaultShader = new TesselationShaderProgram(dev, devcon,
                                                                           L"hs.hlsl",
                                                                           "VShader", "PShader",
                                                                           "HShader", "DShader");
    Texture* bricksTexture = new Texture(dev, devcon, "grass_a.jpg", 0);
    Texture* bricksTexture_n = new Texture(dev, devcon, "grass_n.jpg", 1);
    Texture* arr[2] = {bricksTexture, bricksTexture_n};

    Texture* displacementMap = new Texture(dev, devcon, "heightmap.png", 0);
    Texture* terrainNormalMap = new Texture(dev, devcon, "normalmap.png", 1);
    Texture* d[2] = {displacementMap, terrainNormalMap};

    //material = new DefaultMaterial(dev, devcon, defaultShader, 2, arr);
    material = new TesselationDefaultLitMaterial(dev, devcon, defaultShader, 3, arr, 2,  d);
    ObjLoader::Load(dev, devcon, L"plane.obj", &pVBuffer, &pIBuffer);

    dirLight = new DirectionLight();
    dirLight->SetDirection(XMFLOAT3(0.3f, -0.5f, -1.0f));
    dirLight->SetColor(XMFLOAT3(0.5f, 0.5f, 0.5f),
                       XMFLOAT3(0.08f, 0.08f, 0.08f), XMFLOAT3(0.1f, 0.1f, 0.1f));

    pointLight = new PointLight();
    pointLight->SetPosition(XMFLOAT3(0.1f, 0.1f, -0.45f));
    pointLight->SetColor(XMFLOAT3(1.0f, 1.0f, 1.0f),
                       XMFLOAT3(0.08f, 0.08f, 0.08f), XMFLOAT3(0.8f, 0.8f, 0.8f));
    pointLight->SetAttenuationCoefficients(0.5f, 0.8f);

    material->SetDirLightSource(dirLight);
    material->AddPointLightSource(pointLight);

    skybox = new Skybox(dev, devcon, L"skybox.dds");
    player = new FirstPlayerController(camera);
}

void D3D_Render(float dt){
    player->Update(dt);

    XMFLOAT4 bgColor = XMFLOAT4(0.0f, 0.2f, 0.4f, 1.0f);

    devcon->ClearRenderTargetView(backbuffer, (const float*)&bgColor);
    devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
                                  1.0f, 0);

    skybox->Render(devcon, camera);

    devcon->RSSetState(rasterizerState);
    devcon->OMSetDepthStencilState(pDSState, 0);
    //angle += dt * 30.0f;
    XMMATRIX model = XMMatrixMultiply(XMMatrixRotationY(XMConvertToRadians(angle)), XMMatrixScaling(1.0f, 1.0f, 1.25f));
    XMStoreFloat4x4(&transform, model);
    material->Render(camera, transform);

    UINT vSize = sizeof(VERTEX);
    UINT vOffset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &vSize, &vOffset);
    devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

    devcon->DrawIndexed(2646, 0, 0);
    //15420

    swapchain->Present(1, 0);
}

void D3D_Clean(){
    delete material;
    delete camera;
    delete player;
    delete skybox;

    pDSState->Release();
    depthStencilView->Release();
    depthStencilBuffer->Release();
    rasterizerState->Release();
    pVBuffer->Release();
    pIBuffer->Release();
    swapchain->Release();
    dev->Release();
    devcon->Release();
    backbuffer->Release();
}

void D3D_SetViewport(int width, int height){
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    devcon->RSSetViewports(1, &viewport);
}

void InitPipeline(){
//    D3D11_INPUT_ELEMENT_DESC ied[] = {
//            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
//                    0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
//                    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
//    };

}

void InitVertexBuffer(){
//    VERTEX vertices[] = {
//      {XMFLOAT3(-0.6f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
//      {XMFLOAT3(0.6f, 0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
//      {XMFLOAT3(0.6f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
//      {XMFLOAT3(-0.6f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
//    };
    VERTEX vertices[] = {
            {XMFLOAT3(-0.8f, 0.8f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
            {XMFLOAT3(0.8f, 0.8f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
            {XMFLOAT3(0.8f, -0.8f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
            {XMFLOAT3(-0.8f, -0.8f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f)},
    };


    UINT indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    D3D11_BUFFER_DESC vBD;
    ZeroMemory(&vBD, sizeof(vBD));

    vBD.Usage = D3D11_USAGE_DYNAMIC;
    vBD.ByteWidth = sizeof(vertices);
    vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&vBD, NULL, &pVBuffer);

    D3D11_MAPPED_SUBRESOURCE vMs;
    devcon->Map(pVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vMs);
    memcpy(vMs.pData, vertices, sizeof(vertices));
    devcon->Unmap(pVBuffer, NULL);


    D3D11_BUFFER_DESC iBD;
    ZeroMemory(&iBD, sizeof(D3D11_BUFFER_DESC));
    iBD.Usage = D3D11_USAGE_DEFAULT;
    iBD.ByteWidth = sizeof(indices);
    iBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iBD.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    dev->CreateBuffer(&iBD, &InitData, &pIBuffer);

//    D3D11_MAPPED_SUBRESOURCE iMs;
//    devcon->Map(pIBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &iMs);
//    memcpy(iMs.pData, indices, sizeof(indices));
//    devcon->Unmap(pIBuffer, NULL);
}