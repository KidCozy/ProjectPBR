#include "RenderManager.h"

void RenderManager::SetInputElements()
{

	DefaultInputLayout.push_back(
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 } );
	DefaultInputLayout.push_back(
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });


}

RenderManager::RenderManager(ID3D11Device * InDevice, ID3D11DeviceContext * InContext, UINT BufferCount)
{
	Device = InDevice;
	Context = InContext;

	

}

void RenderManager::OnInit()
{
	SetInputElements();

	StaticSphere.SetProperty(3.0f, 100, 100);
	StaticSphere.Init();

	StaticSphere.GetMaterial()->CompileShader(Context, Device, DefaultInputLayout, L"DefaultShader.fx");
	D3DHelper::AllocConstantBuffer(Device, StaticSphere.GetBuffer(), StaticSphere.GetVertices(), StaticSphere.GetIndices());
	
	Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void RenderManager::OnUpdate()
{
	static const UINT Stride = sizeof(Vertex);
	static const UINT Offset = 0;

	StaticSphere.GetMaterial()->GetConstBuffer()->View = XMMatrixTranspose(StaticCamera.GetView());
	StaticSphere.GetMaterial()->GetConstBuffer()->Projection = XMMatrixTranspose(StaticCamera.GetProjection());

	Context->VSSetShader(*StaticSphere.GetMaterial()->GetVertexShader(), NULL, 0);
	Context->VSSetConstantBuffers(0, 1, &StaticSphere.GetBuffer()->ABuffer);
	Context->IASetVertexBuffers(0, 1, &StaticSphere.GetBuffer()->VBuffer, &Stride, &Offset);
	Context->IASetIndexBuffer(StaticSphere.GetBuffer()->IBuffer, DXGI_FORMAT_R16_UINT, 0);
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
