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

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
};

PSInput VS(VSInput Input)
{
    PSInput Output = (PSInput) 0;

    Output.Position = float4(Input.Position, 1.0f);
    Output.Position = mul(Output.Position, World);
    Output.Position = mul(Output.Position, View);
    Output.Position = mul(Output.Position, Projection);

    Output.Normal = normalize(Input.Normal);

    return Output;
}

float4 PS(PSInput Input) : SV_Target
{
    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return Color;
}

technique GeometryTech
{
    pass
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};