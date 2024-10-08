#include <sstream>
#include "ObjLoader.h"
//
// Created by kotk4 on 04.09.2024.
//

void ObjLoader::Load(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCWSTR fileName,
                     ID3D11Buffer** ppVerticesBuf, ID3D11Buffer** ppIndicesBuf)
{
    std::vector<XMFLOAT3> rawVerticesPosition;
    std::vector<XMFLOAT2> rawVerticesTexCoord;
    std::vector<XMFLOAT3> rawVerticesNormal;
    std::vector<std::array<UINT, 3>> rawVertices;

    std::vector<VERTEX> vertices;
    std::vector<UINT> indices;

    wchar_t currentChar;

    std::wifstream fileIn(fileName);

    while(fileIn){
        currentChar = fileIn.get();

        switch(currentChar) {
            case '#':
                do{
                    currentChar = fileIn.get();
                }while(currentChar != '\n');
                break;
            case 'm':
                do{
                    currentChar = fileIn.get();
                }while(currentChar != '\n');
                break;
            case 'o':
                do{
                    currentChar = fileIn.get();
                }while(currentChar != '\n');
                break;
            case 'g':
                do{
                    currentChar = fileIn.get();
                }while(currentChar != '\n');
                break;
            case 'u':
                do{
                    currentChar = fileIn.get();
                }while(currentChar != '\n');
                break;
            case 's':
                do{
                    currentChar = fileIn.get();
                }while(currentChar != '\n');
                break;
            case 'v':
                currentChar = fileIn.get();

                if(currentChar == ' '){
                    float x, y, z;
                    fileIn >> x >> y >> z;
                    rawVerticesPosition.push_back(XMFLOAT3(x, y, z * -1.0f));
                }
                if(currentChar == 't'){
                    float u, v;
                    fileIn >> u >> v;
                    v = 1 - v;
                    rawVerticesTexCoord.push_back(XMFLOAT2(u, v));
                }
                if(currentChar == 'n'){
                    float x, y, z;
                    fileIn >> x >> y >> z;
                    rawVerticesNormal.push_back(XMFLOAT3(x, y, z * -1.0f));
                }
                break;
            case 'f':
                currentChar = fileIn.get();
                if (currentChar != ' ') break;

                std::wstring faceStr;
                std::wstring vertStr;

                while(currentChar != '\n'){
                    currentChar = fileIn.get();
                    faceStr += currentChar;
                }

                if(faceStr.empty()) break;

                std::wstringstream faceStream(faceStr);

                for(int i = 0; i < 3; i++){
                    std::wstring vertPart;
                    int partNum = 0;
                    UINT v, vt, vn = 0;

                    faceStream >> vertStr;

                    for(int j = 0; j < vertStr.length(); j++){
                        if(vertStr[j] != '/') vertPart += vertStr[j];

                        if(vertStr[j] == '/' || j == (vertStr.length() - 1)){
                            std::wistringstream wstringToInt(vertPart);

                            if(partNum == 0){
                                wstringToInt >> v;
                                v--;
                            }
                            if(partNum == 1){
                                if(!vertPart.empty()){
                                    wstringToInt >> vt;
                                    vt--;
                                }
                            }
                            if(partNum == 2){
                                wstringToInt >> vn;
                                vn--;
                            }

                            vertPart = L"";
                            partNum++;
                        }
                    }

                    std::array<UINT, 3> rawVertex = {v, vt ,vn};
                    bool check = false;

                    for(int k = 0; k < rawVertices.size(); k++){
                        if(rawVertices[k] == rawVertex){
                            check = true;
                            indices.push_back(k);
                            break;
                        }
                    }

                    if(!check){
                        rawVertices.push_back(rawVertex);

                        VERTEX vertex{};
                        vertex.position = rawVerticesPosition[v];
                        vertex.texCoord = rawVerticesTexCoord[vt];
                        vertex.normal = rawVerticesNormal[vn];

                        vertices.push_back(vertex);
                        indices.push_back(vertices.size() - 1);
                    }
                }

                CalculateTBN(vertices, indices);

                break;
        }
    }

    D3D11_BUFFER_DESC vBD;
    ZeroMemory(&vBD, sizeof(vBD));

    vBD.Usage = D3D11_USAGE_DYNAMIC;
    vBD.ByteWidth = vertices.size() * sizeof(VERTEX);
    vBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&vBD, NULL, ppVerticesBuf);

    D3D11_MAPPED_SUBRESOURCE vMs;
    devcon->Map(*ppVerticesBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &vMs);
    memcpy(vMs.pData, vertices.data(), vertices.size() * sizeof(VERTEX));
    devcon->Unmap(*ppVerticesBuf, NULL);


    D3D11_BUFFER_DESC iBD;
    ZeroMemory(&iBD, sizeof(D3D11_BUFFER_DESC));
    iBD.Usage = D3D11_USAGE_DEFAULT;
    iBD.ByteWidth = indices.size() * sizeof(UINT);
    iBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
    iBD.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices.data();
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    dev->CreateBuffer(&iBD, &InitData, ppIndicesBuf);

    rawVerticesPosition.clear();
    rawVerticesTexCoord.clear();
    rawVerticesNormal.clear();
}

void ObjLoader::CalculateTBN(std::vector<VERTEX> &vertices, std::vector<UINT> &indices){
    UINT firstIndex, secondIndex, thirdIndex = 0;
    firstIndex = indices[indices.size() - 3];
    secondIndex = indices[indices.size() - 2];
    thirdIndex = indices[indices.size() - 1];

    XMVECTOR deltaPos1, deltaPos2;
    XMVECTOR deltaUv1, deltaUv2;

    deltaPos1 = XMVectorSubtract(XMLoadFloat3(&vertices[secondIndex].position),
                                 XMLoadFloat3(&vertices[firstIndex].position));
    deltaPos2 = XMVectorSubtract(XMLoadFloat3(&vertices[thirdIndex].position),
                                 XMLoadFloat3(&vertices[firstIndex].position));

    deltaUv1 = XMVectorSubtract(XMLoadFloat2(&vertices[secondIndex].texCoord),
                                XMLoadFloat2(&vertices[firstIndex].texCoord));
    deltaUv2 = XMVectorSubtract(XMLoadFloat2(&vertices[thirdIndex].texCoord),
                                XMLoadFloat2(&vertices[firstIndex].texCoord));
    deltaUv1 = XMVectorMultiply(deltaUv1, XMVectorSet(1.0f, -1.0f, 1.0f, 1.0f));
    deltaUv2 = XMVectorMultiply(deltaUv2, XMVectorSet(1.0f, -1.0f, 1.0f, 1.0f));

    XMMATRIX posMat = XMMATRIX(deltaPos1, deltaPos2,
                               XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                               XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
    XMMATRIX uvsMat = XMMATRIX(deltaUv1, deltaUv2,
                               XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
                               XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
    XMMATRIX uvsInvMat = XMMatrixInverse(NULL, uvsMat);
    XMMATRIX btMat = XMMatrixMultiply(uvsInvMat, posMat);

    XMStoreFloat3(&vertices[firstIndex].tangent, XMVector3Normalize(btMat.r[0]));
    XMStoreFloat3(&vertices[firstIndex].bitangent, XMVector3Normalize(btMat.r[1]));

    XMStoreFloat3(&vertices[secondIndex].tangent, XMVector3Normalize(btMat.r[0]));
    XMStoreFloat3(&vertices[secondIndex].bitangent, XMVector3Normalize(btMat.r[1]));

    XMStoreFloat3(&vertices[thirdIndex].tangent, XMVector3Normalize(btMat.r[0]));
    XMStoreFloat3(&vertices[thirdIndex].bitangent, XMVector3Normalize(btMat.r[1]));
}
