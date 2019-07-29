#include "RenderManager.h"

void RenderManager::SetMaterialPath()
{
	Material* ArrayContainer[DEFAULT_MATERIALSIZE];

	for (UINT i = 0; i < DEFAULT_MATERIALSIZE; i++)
	{
		ArrayContainer[i] = new Material();
	}

	ArrayContainer[0]->SetFile(L"DefaultShader.fx");
	ArrayContainer[1]->SetFile(L"ScreenQuadShader.fx");

	MaterialContainer.push_back(ArrayContainer[0]);
	MaterialContainer.push_back(ArrayContainer[1]);

}

void RenderManager::BindBuffer(Geometry * Geometry)
{
	static const UINT Stride = sizeof(Vertex);
	static const UINT Offset = 0;

	Context->IASetVertexBuffers(0, 1, &Geometry->GetBuffer()->VBuffer, &Stride, &Offset);
	Context->IASetIndexBuffer(Geometry->GetBuffer()->IBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void RenderManager::DrawObject(Geometry * Object)
{
	Object->GetMaterial()->SetWorldMatrix(XMLoadFloat4x4(&Object->GetTransform()->GetTransform()));
	Object->GetMaterial()->SetViewMatrix(StaticCamera.GetView());
	Object->GetMaterial()->SetProjectionMatrix(StaticCamera.GetProjection());

	//if (FAILED(StaticSphere.GetMaterial()->GetEffect()->GetTechniqueByName("GeometryTech")->GetPassByIndex(0)->Apply(0, Context)))
	//{
	//	MessageBox(NULL, L"Apply failed.", 0, 0);
	//}

	BindBuffer(Object);

	Context->DrawIndexed(Object->GetIndexCount(), 0, 0);


}

void RenderManager::SetPass(Geometry& Object, UINT Index)
{
	ID3DX11EffectPass* Pass = Object.GetMaterial()->GetPass(Index);

	if (FAILED(Pass->Apply(0, Context)))
	{
		MessageBox(NULL, L"Apply Failed.", 0, 0);
	}

}

RenderManager::RenderManager(UINT InWidth, UINT InHeight, ID3D11Device * InDevice, ID3D11DeviceContext * InContext, UINT BufferCount)
{
	Device = InDevice;
	Context = InContext;

	Width = InWidth;
	Height = InHeight;

}

void RenderManager::PostInitialize()
{
	SetMaterialPath();
	D3DHelper::CompileShader(Device, MaterialContainer.data()[0]);
	D3DHelper::CompileShader(Device, MaterialContainer.data()[1]);

	Context->OMSetRenderTargets(BUFFERCOUNT, &GBuffer[0].RTV, DepthStencilView);
}

void RenderManager::OnInit()
{
	SetTechnique(MaterialContainer.data()[0]->GetEffect()->GetTechniqueByName("GeometryTech"));

	StaticSphere.SetMaterial(MaterialContainer.data()[0]);
	StaticSphere.SetProperty(1.0f, 64, 64);
	StaticSphere.Init();

	ScreenQuad.SetMaterial(MaterialContainer.data()[0]);
	ScreenQuad.Init();

	D3DHelper::AllocConstantBuffer(Device, &StaticSphere);
	D3DHelper::AllocConstantBuffer(Device, &ScreenQuad);

	Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	World = XMMatrixIdentity();

}

void RenderManager::OnUpdate()
{
	SetPass(StaticSphere, 0);
	StaticSphere.GetMaterial()->SetWorldMatrix(XMMatrixIdentity());
}

void RenderManager::OnRender()
{
	DrawObject(&StaticSphere);

	SetPass(ScreenQuad, 1);
	ScreenQuad.GetMaterial()->SetWorldMatrix(XMMatrixIdentity());
	DrawObject(&ScreenQuad);
}

void RenderManager::OnRelease()
{
	StaticSphere.Release();
	ScreenQuad.Release();
}
