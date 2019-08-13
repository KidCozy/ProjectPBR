SamplerState SampleState : register(s0);

Texture2D PositionBuffer;
Texture2D NormalBuffer;
Texture2D BinormalBuffer;
Texture2D TangentBuffer;
Texture2D DepthBuffer;
Texture2D ColorBuffer;
Texture2D AlbedoBuffer;

TextureCube CubeSlot;

float2 PixelOffset;
float3 ViewDirection;
float Roughness = 1.0f;

cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
};

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Binormal : BINORMAL;
    float3 Tangent : TANGENT;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR;
};

struct RTInput
{
    float4 hPos : HPOS;
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
    float3 Binormal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float2 UV : TEXCOORD3;
    float4 Color : TEXCOORD4;
    float4 Albedo : TEXCOORD5;
    float3 Reflection : TEXCOORD6;

};

struct PSInput
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Binormal : SV_Target2;
    float4 Tangent : SV_Target3;
    float4 Color : SV_Target4;
    float4 Albedo : SV_Target5;
    float4 Reflection : SV_Target6;
};

struct PSFinal
{
    float4 Color : SV_Target;
};

/*********************************/
struct VSLineInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct ColorOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};
/*********************************/

float GaussFilter5x5[5][5] =
{
    1, 4, 6, 4, 1,
   4, 16, 24, 16, 4,
   6, 24, 36, 24, 6,
   4, 16, 24, 16, 4,
   1, 4, 6, 4, 1
};

float GaussFilter13x13[13][13] =
{
    0.005915, 0.005916,	0.005916,	0.005916,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005916,	0.005916,	0.005916,	0.005915,
0.005916,	0.005916,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005916,	0.005916,
0.005916,	0.005917,	0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,	0.005917,	0.005916,
0.005916,	0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,	0.005916,
0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,
0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,
0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,
0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,
0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,
0.005916,	0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,	0.005916,
0.005916,	0.005917,	0.005917,	0.005917,	0.005918,	0.005918,	0.005918,	0.005918,	0.005918,	0.005917,	0.005917,	0.005917,	0.005916,
0.005916,	0.005916,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005916,	0.005916,
0.005915,	0.005916,	0.005916,	0.005916,	0.005917,	0.005917,	0.005917,	0.005917,	0.005917,	0.005916,	0.005916,	0.005916,	0.005915
  
};

RTInput VS(VSInput Input)
{
    RTInput Output = (RTInput) 0;

    Output.hPos = float4(Input.Position, 1.0f);
    Output.Position = float4(Input.Position, 1.0f);
    Output.Position = mul(Output.Position, World);
    Output.Position = mul(Output.Position, View);
    Output.Position = mul(Output.Position, Projection);

    Output.Normal = mul(Input.Normal, (float3x3) World);
    Output.Normal = normalize(Output.Normal);

    Output.Binormal = mul(Input.Binormal, (float3x3) World);
    Output.Binormal = normalize(Output.Binormal);

    Output.Tangent = mul(Input.Tangent, (float3x3) World);
    Output.Tangent = normalize(Output.Tangent);

    Output.UV = Input.UV;
    Output.Color = Input.Color;

    float3 viewDir = normalize(ViewDirection);

    return Output;
}

ColorOutput BasicVS(VSLineInput Input)
{
    ColorOutput Output = (ColorOutput) 0;
    Output.Position = float4(Input.Position, 1.0f);
    Output.Position = mul(Output.Position, World);
    Output.Position = mul(Output.Position, View);
    Output.Position = mul(Output.Position, Projection);
    Output.Color = float4(Input.Color, 1.0f);

    return Output;
}

PSInput RTWriter(RTInput Input)
{
    PSInput Output = (PSInput)0;

    Output.Position = float4(Input.hPos.xyz, 1.0f);
    Output.Normal = float4(Input.Normal, 1.0f);
    Output.Binormal = float4(Input.Binormal, 1.0f);
    Output.Tangent = float4(Input.Tangent, 1.0f);
    Output.Reflection = float4(Input.Reflection, 1.0f);
    
    Output.Color = Input.Color;
    
    Output.Albedo = CubeSlot.Sample(SampleState, Output.Normal.rgb);

    return Output;
}

PSInput LineRTWriter(RTInput Input)
{
    PSInput Output = (PSInput) 0;

    Output.Color = Input.Color;

    return Output;
}

RTInput DeferredVS(VSInput Input)
{
    RTInput Output = (RTInput) 0;

    Output.hPos = float4(Input.Position, 1.0f);
    Output.Position = Output.hPos;
    Output.Normal = Input.Normal;
    Output.Binormal = Input.Binormal;
    Output.Tangent = Input.Tangent;
    Output.UV = Input.UV;
    Output.Color = Input.Color;

    return Output;
}

float3 GaussBlur(float3 Pixel, float2 UV)
{
    float3 GaussColor = 0;
    
    for (int y = -6; y <= 6; ++y)
    {
        for (int x = -6; x <= 6; ++x)
        {
            float2 Offset = float2(x, y) * PixelOffset;
            float3 NormalPixel = NormalBuffer.Sample(SampleState, UV + Offset).rgb;
            GaussColor += (GaussFilter13x13[y + 6][x + 6]) * NormalPixel;
        }
    }
    return GaussColor;

}


PSFinal DeferredPS(RTInput Input)
{
    PSFinal Output = (PSFinal)0;

    float3 Position = PositionBuffer.Sample(SampleState, Input.UV).rgb;
    float3 Albedo = AlbedoBuffer.Sample(SampleState, Input.UV).rgb;

    float3 Normal = NormalBuffer.Sample(SampleState, Input.UV).rgb;
    float3 Binormal = BinormalBuffer.Sample(SampleState, Input.UV).rgb;
    float3 Tangent = TangentBuffer.Sample(SampleState, Input.UV).rgb;

    float3 Color = Input.Color.rgb;
    float3 LightDir = normalize(float3(1.0f, 1.0f, 1.0f));

    float3 BlurColor = 0;
    float3 FinalColor = Normal;
    float3 Specular = 0;
    float3 ViewDir = -normalize(ViewDirection);

    float3 Half = normalize(LightDir + ViewDir);

    float NE = dot(Normal, ViewDir);
    float NL = dot(Normal, LightDir);
    float NH = dot(Normal, Half);
    float VH = dot(ViewDir, Half);
    float LH = dot(LightDir, Half);
    float NV = dot(Normal, ViewDir);

    float G = min(1, min(2*NH*NE/VH, 2*NH*NL/VH));



    float NH2 = NH * NH;

    float D = exp(-(1 - NH2) / (NH2 * Roughness * Roughness)) / (4 * Roughness * Roughness * NH2 * NH2);

    float N = 20.0f;
    float g = sqrt(N * N + LH * LH - 1);
    float gpc = g + LH;
    float gnc = g - LH;
    float cgpc = LH * gpc - 1;
    float cgnc = LH * gnc + 1;
    float F = 0.5f * gnc * gnc * (1 + cgpc * cgpc / (cgnc * cgnc)) / (gpc * gpc);

    float4 ks = { 1.0f,1.0f,1.0f, 1.0f };

  //  BlurColor = GaussBlur(Normal, Input.UV);
  
    if (length(BlurColor) > 0.0f)
        FinalColor = BlurColor;


    float3 Ambient = float3(0.1f, 0.1f, 0.1f);
    if (length(Normal) > 0.0f)
    {
        float3 lightDir = normalize(LightDir);
        float3 luminance = saturate(dot(LightDir, FinalColor));
        FinalColor = luminance;
    }

    Input.Reflection = reflect(LightDir, Normal);
    Input.Reflection = normalize(Input.Reflection);

    if (FinalColor.x > 0)
    {
        Specular = saturate(dot(Input.Reflection, -(normalize(ViewDirection))));
        Specular = pow(Specular, 10.0f);

    }
    
    float3 Final = 0;
    Final += ks.rgb * max(0, F * D * G / NV);
    Output.Color = float4(Albedo+Final, 1.0f);
    
    return Output;
}

PSFinal NormalBufferPS(RTInput Input)
{
    PSFinal Output = (PSFinal) 0;

    float3 Normal = NormalBuffer.Sample(SampleState, Input.UV).rgb;
    Output.Color = float4(Normal, 1.0f);
    
    return Output;
}

PSFinal BinormalBufferPS(RTInput Input)
{
    PSFinal Output = (PSFinal) 0;

    float3 Binormal = BinormalBuffer.Sample(SampleState, Input.UV).rgb;
    Output.Color = float4(Binormal, 1.0f);
    
    return Output;
}

PSFinal TangentBufferPS(RTInput Input)
{
    PSFinal Output = (PSFinal) 0;

    float3 Tangent = TangentBuffer.Sample(SampleState, Input.UV).rgb;
    Output.Color = float4(Tangent, 1.0f);
    
    return Output;
}

PSFinal DepthBufferPS(RTInput Input)
{
    PSFinal Output = (PSFinal) 0;

    float3 Depth = DepthBuffer.Sample(SampleState, Input.UV).rgb;
    Output.Color = float4(Depth, 1.0f);
    
    return Output;
}


float4 ForwardPS(ColorOutput Input) : SV_Target
{
    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return Color;
}

technique11 GeometryTech
{
    // G buffer Rendering
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, RTWriter()));
    }

    // Merge buffer Rendering (Screen Quad only.)
    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, DeferredVS()));
        SetPixelShader(CompileShader(ps_5_0, DeferredPS()));
    }

    // For debugging only (Don't use this pass for release)
    pass Forward0
    {
        SetVertexShader(CompileShader(vs_5_0, BasicVS()));
        SetPixelShader(CompileShader(ps_5_0, ForwardPS()));
    }

    pass LinePass
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, LineRTWriter()));
    }

};

technique11 BufferTech
{
    pass NormalPass0
    {
        SetVertexShader(CompileShader(vs_5_0, DeferredVS()));
        SetPixelShader(CompileShader(ps_5_0, NormalBufferPS()));
    }

    pass BinormalPass
    {
        SetVertexShader(CompileShader(vs_5_0, DeferredVS()));
        SetPixelShader(CompileShader(ps_5_0, BinormalBufferPS()));
    }

    pass TangentPass
    {
        SetVertexShader(CompileShader(vs_5_0, DeferredVS()));
        SetPixelShader(CompileShader(ps_5_0, TangentBufferPS()));
    }

    pass DepthPass
    {
        SetVertexShader(CompileShader(vs_5_0, DeferredVS()));
        SetPixelShader(CompileShader(ps_5_0, DepthBufferPS()));
    }
};