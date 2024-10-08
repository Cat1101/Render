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
Texture2D normal1 : register(t2);
SamplerState normalSample1 : register(s2);

Texture2D displacement : register(t0);
SamplerState displacementSample : register(s0);
Texture2D terrainNormal : register(t1);
SamplerState terrainNormalSample : register(s1);

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
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
    float3x3 tbn : TBN;
};

struct PatchTess
{
    float Edges[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
    float3x3 tbn : TBN;
};

struct DS_OUTPUT
{
    float4 vCameraPosition : SV_POSITION;
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD;
    float3 viewDir : VIEWDIR;
    float3x3 tbn : TBN;
};

VS_OUTPUT VShader(VS_INPUT input)
{
     VS_OUTPUT vout;

     float3 T = normalize(mul(modelMatrix, float4(input.vTangent, 0.0f)));
     float3 B = normalize(mul(modelMatrix, float4(input.vBitangent, 0.0f)));
     float3 N = normalize(mul(modelMatrix, float4(input.vNormal, 0.0f)));

     float3x3 TBN = float3x3(T, B, N);

     float4 modelPos = mul(modelMatrix, input.vPosition);

     vout.vPosition = modelPos.xyz;
     vout.vTexCoord = input.vTexCoord * 1;
     vout.tbn = TBN;

     return vout;
}

PatchTess PatchHS(InputPatch<VS_OUTPUT, 3> patch, uint patchID : SV_PrimitiveID)
{
    PatchTess pt;

    pt.Edges[0] = 64;
    pt.Edges[1] = 64;
    pt.Edges[2] = 64;
    pt.Inside = 64;

    return pt;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
[maxtessfactor(64.0)]
HS_OUTPUT HShader(InputPatch<VS_OUTPUT, 3> patch, uint id : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
    HS_OUTPUT hout;

    hout.vPosition = patch[id].vPosition;
    hout.vTexCoord = patch[id].vTexCoord;
    hout.tbn = patch[id].tbn;

    return hout;
}

[domain("tri")]
DS_OUTPUT DShader(PatchTess patchTess, float3 bary : SV_DomainLocation, OutputPatch<HS_OUTPUT, 3> tri)
{
    DS_OUTPUT dout;
    float3 n, t, b;

    dout.vPosition = bary.x * tri[0].vPosition + bary.y * tri[1].vPosition + bary.z * tri[2].vPosition;
    dout.tbn = bary.x * tri[0].tbn + bary.y * tri[1].tbn + bary.z * tri[2].tbn;
    dout.vTexCoord = bary.x * tri[0].vTexCoord + bary.y * tri[1].vTexCoord + bary.z * tri[2].vTexCoord;
    dout.viewDir = normalize(-viewDir);

    t = normalize(float3(dout.tbn[0][0], dout.tbn[0][1], dout.tbn[0][2]));
    b = normalize(float3(dout.tbn[1][0], dout.tbn[1][1], dout.tbn[1][2]));
    n = normalize(float3(dout.tbn[2][0], dout.tbn[2][1], dout.tbn[2][2]));

    dout.tbn = float3x3(t, b, n);

    n = normalize(terrainNormal.SampleLevel(terrainNormalSample, dout.vTexCoord, 0).xyz * 2.0f - 1.0f);
    n = mul(transpose(dout.tbn), n);

    float h = displacement.SampleLevel(displacementSample, dout.vTexCoord, 0).r;
    dout.vPosition.y = dout.vPosition.y - 1 + h * 1.5f;

    float3 dP1 = tri[1].vPosition - tri[0].vPosition;
    float3 dP2 = tri[2].vPosition - tri[0].vPosition;

    float2 dUv1 = tri[1].vTexCoord - tri[0].vTexCoord;
    float2 dUv2 = tri[2].vTexCoord - tri[0].vTexCoord;

    n = normalize(n);
    float3 dp2perp = cross(dP2, n);
    float3 dp1perp = cross(n, dP1);
    t = dp2perp * dUv1.x + dp1perp * dUv2.x;
    b = dp2perp * dUv1.y + dp1perp * dUv2.y;

    float invmax = rsqrt(max(dot(t,t), dot(b,b)));

    t = normalize(t * invmax * -1.0f);
    b = normalize(b * invmax);

    dout.tbn = float3x3(t, b, n);

    dout.vCameraPosition = mul(cameraMatrix, float4(dout.vPosition.xyz, 1.0f));

    return dout;
}

float4 PShader(float4 cameraPosition : SV_POSITION, float3 position : POSITION, float2 vTexCoord : TEXCOORD, float3 viewDir : VIEWDIR, float3x3 tbn : TBN) : SV_TARGET
{
    float3 n1 = normalize((normal.Sample(normalSample, vTexCoord).xyz * 2.0) - 1.0);
    float3 n2 = normalize((normal1.Sample(normalSample1, vTexCoord).xyz * 2.0) - 1.0);
    float3 r = normalize(float3(n1.xy + n2.xy, n1.z*n2.z));

    float3 n = r * 0.5 + 0.5;
    n = normalize(mul(transpose(tbn), n1));
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
    //return float4(n, 1.0f);
    //return float4(tbn[2][0], tbn[2][1], tbn[2][2], 1.0f);
    return albedo.Sample(albedoSample, vTexCoord) * (pl + dirl);
}