//
// Created by kotk4 on 31.08.2024.
//

#ifndef RENDER_CONSTANTBUFFER_H
#define RENDER_CONSTANTBUFFER_H

#include "d3d11.h"
#include "ShaderProgram.h"

template<typename T>
class ConstantBuffer{
private:
    ID3D11Buffer* m_cBuffer = NULL;

    T m_data;
public:
    ConstantBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon);
    ~ConstantBuffer();

    ID3D11Buffer* Get();

    void SetData(ID3D11DeviceContext* devcon, T data);
};


template<typename T>
ConstantBuffer<T>::ConstantBuffer(ID3D11Device* dev, ID3D11DeviceContext* devcon) {
    ZeroMemory(&m_data, sizeof(T));

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(T);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&bd, NULL, &m_cBuffer);

    SetData(devcon, m_data);
}

template<typename T>
ConstantBuffer<T>::~ConstantBuffer() {
    m_cBuffer->Release();
}

template<typename T>
void ConstantBuffer<T>::SetData(ID3D11DeviceContext* devcon, T data) {
    UINT a = sizeof(T);

    D3D11_MAPPED_SUBRESOURCE vMs;
    devcon->Map(m_cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vMs);
    memcpy(vMs.pData, &data, sizeof(T));
    devcon->Unmap(m_cBuffer, NULL);
}

template<typename T>
ID3D11Buffer* ConstantBuffer<T>::Get() {
    return m_cBuffer;
}

#endif //RENDER_CONSTANTBUFFER_H
