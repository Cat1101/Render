cbuffer cbPerFrame
{
    float4x4 cameraMatrix;
    float4x4 modelMatrix;
    float3 viewDir;
};

TextureCube cubemap;
SamplerState cubemapSample : register(s0);

SamplerState samTriLinearSam
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VertexIn
{
     float4 vPosition : POSITION;
     float2 vTexCoord : TEXCOORD;
};

struct VertexOut
{
    float4 vCameraPosition : SV_POSITION;
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.vCameraPosition = mul(cameraMatrix, float4(vin.vPosition.xyz, 1.0f)).xyww;

    vout.vPosition = vin.vPosition;
    vout.vTexCoord = vin.vTexCoord;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return cubemap.Sample(samTriLinearSam, pin.vPosition);
}