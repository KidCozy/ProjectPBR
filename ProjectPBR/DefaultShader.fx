SamplerState SampleState : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct RTInput
{
    float4 hPos : HPOS;
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
    float2 RenderTarget : TEXCOORD1;

    
};

struct PSInput
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;
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
    PSInput Output;

    Output.Position = mul(Input.hPos, World);
    Output.Normal = float4(Input.Normal, 1.0f);

}


float4 PS(PSInput Input) : SV_Target
{
    float4 Color = float4(0.0f, 1.0f, 0.0f, 1.0f);

    return Color;
}

technique11 GeometryTech
{
    pass
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};