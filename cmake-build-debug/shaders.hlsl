struct VS_INPUT
{
    float4 vPosition : POSITION;
    float4 vColor : COLOR;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float4 vColor : COLOR;
};

VS_OUTPUT VShader(VS_INPUT input)
{
     VS_OUTPUT vout;

     vout.vPosition = input.vPosition;
     vout.vColor = input.vColor;

     return vout;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}