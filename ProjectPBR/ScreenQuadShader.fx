Texture2D RenderTarget;

cbuffer ConstantBuffer : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

struct VSInput
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

struct PSOutput
{
    float4 Position : SV_Target0;
    float4 Normal : SV_Target1;
};

PSInput VS(VSInput Input)
{
    PSInput Output = (PSInput) 0;

    Output.Position = float4(Input.Position, 1.0f);
    Output.UV = Input.UV;

    return Output;
}

float4 PS(PSInput Input) : SV_Target
{
  //  float4 Color = tex2D(RenderTarget, Input.UV);
    float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Color;
}