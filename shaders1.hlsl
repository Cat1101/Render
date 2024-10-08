Texture2D albedo;
SamplerState albedoSample;

struct VS_INPUT
{
    float4 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD;
};

VS_OUTPUT VShader(VS_INPUT input)
{
     VS_OUTPUT vout;

     vout.vPosition = input.vPosition;
     vout.vTexCoord = input.vTexCoord;

     return vout;
}

float4 PShader(float4 position : SV_POSITION, float2 vTexCoord : TEXCOORD) : SV_TARGET
{
    return albedo.Sample(albedoSample, vTexCoord);
}