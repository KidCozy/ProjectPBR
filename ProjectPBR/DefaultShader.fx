SamplerState SampleState : register(s0);

Texture2D PositionBuffer;
Texture2D NormalBuffer;

float2 PixelOffset;

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
    float2 UV : TEXCOORD0;
};

struct RTInput
{
    float4 hPos : HPOS;
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
    float2 UV : TEXCOORD1;
};

struct PSInput
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;

};

struct PSFinal
{
    float4 Color : SV_Target;
};

struct ColorOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

float GaussFilter5x5[5][5] =
{
    1, 4, 6, 4, 1,
   4, 16, 24, 16, 4,
   6, 24, 36, 24, 6,
   4, 16, 24, 16, 4,
   1, 4, 6, 4, 1
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

    Output.UV = Input.UV;

    return Output;
}

ColorOutput BasicVS(VSInput Input)
{
    ColorOutput Output = (ColorOutput) 0;
    Output.Position = float4(Input.Position, 1.0f);
    Output.Position = mul(Output.Position, World);
    Output.Position = mul(Output.Position, View);
    Output.Position = mul(Output.Position, Projection);

    return Output;

}


PSInput RTWriter(RTInput Input)
{
    PSInput Output = (PSInput)0;

    Output.Position = float4(Input.hPos.xyz, 1.0f);
    Output.Normal = float4(Input.Normal, 1.0f);
    
    return Output;
}

RTInput DeferredVS(VSInput Input)
{
    RTInput Output = (RTInput) 0;

    Output.hPos = float4(Input.Position, 1.0f);
    Output.Position = Output.hPos;
    Output.Normal = Input.Normal;
    Output.UV = Input.UV;

    return Output;
}

PSFinal DeferredPS(RTInput Input)
{
    PSFinal Output = (PSFinal)0;

    float3 Position = PositionBuffer.Sample(SampleState, Input.UV).rgb;

    float3 Normal = NormalBuffer.Sample(SampleState, Input.UV).rgb;

    float3 FinalColor = 0;
    float3 GaussColor = 0;
    
    FinalColor = Normal;

    for (int y = -2; y <= 2; ++y)
    {
        for (int x = -2; x <= 2; ++x)
        {
            float2 Offset = float2(x, y) * PixelOffset;
            float3 NormalPixel = NormalBuffer.Sample(SampleState, Input.UV+Offset).rgb;
            GaussColor += GaussFilter5x5[y + 2][x + 2] * NormalPixel;
            GaussColor /= 3.1f;
        }
    }

    float3 Ambient = float3(0.05f, 0.05f, 0.05f);
    if (length(Normal) > 0.0f)
    {
        float3 lightDir = normalize(float3(1.0f, 1.0f, 1.0f));
        float3 luminance = saturate(dot(lightDir, GaussColor));
        FinalColor = luminance;
    }

    Output.Color = float4(FinalColor, 1.0f);
    
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

    // Merge buffer Rendering
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
};