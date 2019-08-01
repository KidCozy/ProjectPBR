SamplerState SampleState : register(s0);

Texture2D PositionBuffer;
Texture2D NormalBuffer;

cbuffer ConstantBuffer : register(b1)
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
    float4 hPos : POSITION;
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

    return Output;
}

PSInput RTWriter(RTInput Input)
{
    PSInput Output = (PSInput)0;

    Output.Position = float4(Input.hPos.xyz, 1.0f);
    Output.Normal = float4(Input.Normal.xyz, 1.0f);

    return Output;
}

RTInput DeferredVS(VSInput Input)
{
    RTInput Output = (RTInput) 0;

    Output.hPos = float4(Input.Position, 1.0f);
    Output.Position = Output.hPos;
   // Output.Position = normalize(Output.Position);
  //  Output.Position = Output.hPos;
    Output.UV = Input.UV;

    return Output;
}

PSFinal DeferredPS(RTInput Input) : SV_Target
{
    PSFinal Output = (PSFinal)0;
  //  float4 Color; // = PositionBuffer.Sample(SampleState, Input.UV);
 //   Output.Color = Color;
    
    Output.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return Output;
}

PSFinal ForwardPS(RTInput Input) : SV_Target
{
    PSFinal Output = (PSFinal) 0;
   float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    Output.Color = Color;

    return Output;
}

technique11 GeometryTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, RTWriter()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, DeferredVS()));
        SetPixelShader(CompileShader(ps_5_0, DeferredPS()));
    }

    pass Forward0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, ForwardPS()));

    }
};