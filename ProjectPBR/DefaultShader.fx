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
};

struct PSInput
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Binormal : SV_Target2;
    float4 Tangent : SV_Target3;
    float4 Color : SV_Target4;
    float4 Albedo : SV_Target5;
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

    float3 BlurColor = 0;
    float3 FinalColor = Normal;

    //BlurColor = GaussBlur(Normal, Input.UV);
  
    if (length(BlurColor) > 0.0f)
        FinalColor = BlurColor;


    float3 Ambient = float3(0.05f, 0.05f, 0.05f);
    if (length(Normal) > 0.0f)
    {
        float3 lightDir = normalize(float3(1.0f, 1.0f, 1.0f));
        float3 luminance = saturate(dot(lightDir, FinalColor));
        FinalColor = luminance;
    }
    Output.Color = float4(FinalColor+Albedo, 1.0f)+Input.Color;
    
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