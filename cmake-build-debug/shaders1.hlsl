struct DirLight{
    float3 dir;

    float3 ambient;
    float3 diffuse;
    float3 specular;
};

struct PointLight{
    float3 pos;

    float attenuationLinC;
    float attenuationExpC;

    float3 ambient;
    float3 diffuse;
    float3 specular;
};

struct SpotLight{
    float3 pos;
    float3 dir;

    float innerConeAngle;
    float outerConeAngle;

    float3 ambient;
    float3 diffuse;
    float3 specular;
};


cbuffer cbPerObject : register(b0)
{
    float4x4 cameraMatrix;
    float4x4 modelMatrix;
    float3 viewDir;
}

cbuffer lights : register(b1)
{
    DirLight dirLight;
    PointLight pointLights[4];
    SpotLight spotLights[4];
}

Texture2D albedo : register(t0);
SamplerState albedoSample : register(s0);

Texture2D normal : register(t1);
SamplerState normalSample : register(s1);

struct VS_INPUT
{
    float4 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
    float3 vNormal : NORMAL;
    float3 vTangent : TAN;
    float3 vBitangent : BITAN;
};

struct VS_OUTPUT
{
    float4 vCameraPosition : SV_POSITION;
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
    float3 viewDir : VIEWDIR;
    nointerpolation float3x3 tbn : TBN;
};

VS_OUTPUT VShader(VS_INPUT input)
{
     VS_OUTPUT vout;

     float3 T = normalize(mul(modelMatrix, float4(input.vTangent, 0.0f)));
     float3 B = normalize(mul(modelMatrix, float4(input.vBitangent, 0.0f)));
     float3 N = normalize(mul(modelMatrix, float4(input.vNormal, 0.0f)));

     float3x3 TBN = float3x3(T, B, N);

     float4 modelPos = mul(modelMatrix, input.vPosition);

     vout.vCameraPosition = mul(cameraMatrix, modelPos);
     vout.vPosition = modelPos.xyz;
     vout.vTexCoord = input.vTexCoord;
     vout.viewDir = normalize(-viewDir);
     vout.tbn = TBN;

     return vout;
}

float4 PShader(float4 cameraPosition : SV_POSITION, float3 position : POSITION, float2 vTexCoord : TEXCOORD, float3 viewDir : VIEWDIR, float3x3 tbn : TBN) : SV_TARGET
{
    float3 n = normalize((normal.Sample(normalSample, vTexCoord).xyz * 2.0) - 1.0);
    n = mul(transpose(tbn), n);
    float3 lightDir = normalize(-dirLight.dir);

    float3 diffLight = max(0.0, dot(n, lightDir)) * dirLight.diffuse;
    float3 specular = pow(max(dot(viewDir, reflect(-lightDir, n)), 0.0f), 32.0f) * dirLight.specular;

    float3 pointLightsDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 pointLightsSpecular = float3(0.0f, 0.0f, 0.0f);
    float3 pointLightsAmbient = float3(0.0f, 0.0f, 0.0f);

    float3 spotLightsDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 spotLightsSpecular = float3(0.0f, 0.0f, 0.0f);
    float3 spotLightsAmbient = float3(0.0f, 0.0f, 0.0f);

    for(int i = 0; i < 4; i++){
        float3 pointLightDirection = pointLights[i].pos - position;
        float3 pointLightDistance = length(pointLightDirection);
        pointLightDirection = normalize(pointLightDirection);

        float pointLightAttenuation = 1.0f / (1.0f + pointLightDistance * pointLights[i].attenuationLinC
            + pointLightDistance * pointLightDistance * pointLights[i].attenuationExpC);

        pointLightsDiffuse += dot(n, pointLightDirection) * pointLightAttenuation * pointLights[i].diffuse;
        pointLightsSpecular += pow(max(dot(viewDir, reflect(-pointLightDirection, n)), 0.0f), 32.0f) * pointLights[i].specular * pointLightAttenuation;
        pointLightsAmbient += pointLights[i].ambient * pointLightAttenuation;
    }

    for(int i = 0; i < 4; i++){
        float3 spotLightDirection = normalize(spotLights[i].pos - position.xyz);
        float theta = dot(-spotLightDirection, normalize(spotLights[i].dir));
        float epsilon = spotLights[i].innerConeAngle - spotLights[i].outerConeAngle;
        float intensity = clamp((theta - spotLights[i].outerConeAngle) / epsilon, 0.0f, 1.0f);

        spotLightsDiffuse += dot(n, spotLightDirection) * spotLights[i].diffuse * intensity;
        spotLightsSpecular += pow(max(dot(viewDir, reflect(-spotLightDirection, n)), 0.0f), 32.0f) * spotLights[i].specular * intensity;
        spotLightsAmbient += spotLights[i].ambient * intensity;
    }

    float4 dirl = float4((diffLight + dirLight.ambient + specular), 1.0f);
    float4 pl = float4((pointLightsDiffuse + pointLightsAmbient + pointLightsSpecular), 1.0f);
    return albedo.Sample(albedoSample, vTexCoord) * (pl);
}