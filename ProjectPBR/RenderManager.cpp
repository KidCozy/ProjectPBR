#include "RenderManager.h"

void RenderManager::SetInputElements()
{

	DefaultInputLayout.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } );
	DefaultInputLayout.push_back(
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	ScreenQuadInputLayout.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} );
	ScreenQuadInputLayout.push_back(
		{ "TEXCOORD", 0, DXGI_FORMAT_R8G8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
}

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

void RenderManager::BindBuffer(ConstantGeometry * Geometry)
{
	static const UINT Stride = sizeof(Vertex);
	static const UINT Offset = 0;

	Context->IASetVertexBuffers(0, 1, &Geometry->GetBuffer()->VBuffer, &Stride, &Offset);
	Context->IASetIndexBuffer(Geometry->GetBuffer()->IBuffer, DXGI_FORMAT_R16_UINT, 0);
}

RenderManager::RenderManager(ID3D11Device * InDevice, ID3D11DeviceContext * InContext, UINT BufferCount)
{
	Device = InDevice;
	Context = InContext;

	

}

void RenderManager::OnInit()
{
	SetInputElements();
	SetMaterialPath();

	D3DHelper::CompileShader(Device, MaterialContainer.data()[0]);
	D3DHelper::CompileShader(Device, MaterialContainer.data()[1]);

	SetTechnique(MaterialContainer.data()[0]->GetEffect()->GetTechniqueByName("GeometryTech"));

	StaticSphere.SetProperty(3.0f, 100, 100);
	StaticSphere.Init();

	StaticSphere.SetMaterial(MaterialContainer.data()[0]);
//	ScreenQuad.SetMaterial(MaterialContainer.data()[1]);

//	ScreenQuad.Init();

	D3DHelper::AllocConstantBuffer(Device, StaticSphere.GetBuffer(), StaticSphere.GetVertices(), StaticSphere.GetIndices());
	

	Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void RenderManager::OnUpdate()
{
	

	StaticSphere.GetMaterial()->GetConstBuffer()->View = XMMatrixTranspose(StaticCamera.GetView());
	StaticSphere.GetMaterial()->GetConstBuffer()->Projection = XMMatrixTranspose(StaticCamera.GetProjection());

	Context->VSSetShader(*StaticSphere.GetMaterial()->GetVertexShader(), NULL, 0);
	Context->VSSetConstantBuffers(0, 1, &StaticSphere.GetBuffer()->ABuffer);

	Context->PSSetShader(*StaticSphere.GetMaterial()->GetPixelShader(), NULL, 0);



	Context->UpdateSubresource(StaticSphere.GetBuffer()->ABuffer, 0, NULL, StaticSphere.GetMaterial()->GetConstBuffer(), 0, 0);

}

void RenderManager::OnRender()
{
	Context->DrawIndexed(StaticSphere.GetIndexCount(), 0, 0);
}

void RenderManager::OnRelease()
{
	StaticSphere.Release();
}
