//
// Created by kotk4 on 04.09.2024.
//

#ifndef RENDER_OBJLOADER_H
#define RENDER_OBJLOADER_H

#include "vector"
#include "array"
#include "fstream"
#include "d3d11.h"
#include "DirectXMath.h"

using namespace DirectX;

struct VERTEX{
    XMFLOAT3 position;
    XMFLOAT2 texCoord;
    XMFLOAT3 normal;
    XMFLOAT3 tangent;
    XMFLOAT3 bitangent;
};

class ObjLoader{
private:
    static void CalculateTBN(std::vector<VERTEX> &vertices, std::vector<UINT> &indices);
public:
    static void Load(ID3D11Device* dev, ID3D11DeviceContext* devcon, LPCWSTR fileName, ID3D11Buffer** ppVerticesBuf, ID3D11Buffer** ppIndicesBuf);
};

#endif //RENDER_OBJLOADER_H
