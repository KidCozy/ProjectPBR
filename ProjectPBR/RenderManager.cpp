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

void RenderManager::DrawObject(Geometry * Object,TECHNIQUES Technique, RENDERBUFFER RenderBuffer)
{
	static ID3D11RenderTargetView* NullRTV[] = { nullptr };

	Object->GetMaterial()->SetWorldMatrix(XMMatrixIdentity());

	SetPass(*Object,Technique, RenderBuffer);

	Context->OMSetRenderTargets(1, &MergeBuffer, nullptr);

	Object->GetMaterial()->SetWorldMatrix(XMLoadFloat4x4(&Object->GetTransform()->GetTransform()));
	Object->GetMaterial()->SetViewMatrix(StaticCamera.GetView());
	Object->GetMaterial()->SetProjectionMatrix(StaticCamera.GetProjection());

	BindBuffer(Object);

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Context->IASetInputLayout(Object->GetMaterial()->GetInputLayout(0));
	Context->DrawIndexed(Object->GetIndexCount(), 0, 0);
	Context->OMSetRenderTargets(_countof(NullRTV), NullRTV, nullptr);
}

void RenderManager::DrawObject(Geometry * Object)
{
	static ID3D11RenderTargetView* NullRTV[] = { nullptr };

	SetPass(*Object, TECHNIQUE_GEOMETRY, RENDERBUFFER_GEOMETRY);

	Context->OMSetRenderTargets(BUFFERCOUNT, SettingRenderTargets, DepthStencilView);

	Object->GetMaterial()->SetWorldMatrix(XMLoadFloat4x4(&Object->GetTransform()->GetTransform()));
	Object->GetMaterial()->SetViewMatrix(StaticCamera.GetView());
	Object->GetMaterial()->SetProjectionMatrix(StaticCamera.GetProjection());

	BindBuffer(Object);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Context->IASetInputLayout(Object->GetMaterial()->GetInputLayout(0));
	Context->DrawIndexed(Object->GetIndexCount(), 0, 0);
	Context->OMSetRenderTargets(_countof(NullRTV), NullRTV, nullptr);
}

void RenderManager::DrawDebugLine(Geometry * Object, UINT Index)
{
		static ID3D11RenderTargetView* NullRTV[] = { nullptr };

		SetPass(*Object, TECHNIQUE_GEOMETRY, RENDERBUFFER_LINE);

		Context->OMSetRenderTargets(BUFFERCOUNT, SettingRenderTargets, DepthStencilView);

		Object->GetMaterial()->SetWorldMatrix(XMLoadFloat4x4(&Object->GetTransform()->GetTransform()));
		Object->GetMaterial()->SetViewMatrix(StaticCamera.GetView());
		Object->GetMaterial()->SetProjectionMatrix(StaticCamera.GetProjection());

		static const UINT Stride = sizeof(Vertex);
		static const UINT Offset = 0;

		Context->IASetVertexBuffers(0, 1, &Object->GetDebugLine()[Index].Buffer.VBuffer, &Stride, &Offset);
		Context->IASetIndexBuffer(Object->GetDebugLine()[Index].Buffer.IBuffer, DXGI_FORMAT_R16_UINT, 0);

		Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		Context->IASetInputLayout(Object->GetMaterial()->GetInputLayout(0));
		Context->DrawIndexed(Object->GetDebugLine()[Index].LineIndices.size(), 0, 0);
		Context->OMSetRenderTargets(_countof(NullRTV), NullRTV, nullptr);
}

void RenderManager::SetPass(Geometry& Object,TECHNIQUES Technique, UINT Index)
{
	Object.GetMaterial()->SetTechnique(Technique);

	if (FAILED(Object.GetMaterial()->GetPassFromTechnique(Index)->Apply(0, Context)))
	{
		return;
		//MessageBox(NULL, L"Apply Failed.", 0, 0);
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
	//D3DHelper::CompileShader(Device, MaterialContainer.data()[1]);

}

void RenderManager::ClearScreen(XMVECTORF32 ClearColor)
{
	Context->ClearRenderTargetView(MergeBuffer, ClearColor);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (UINT i = 0; i < (BUFFERCOUNT-1); i++)
		Context->ClearRenderTargetView(GBuffer[i].RTV, ClearColor);
}

void RenderManager::OnInit()
{
	StaticSphere.SetMaterial(MaterialContainer.data()[0]);
	StaticSphere.SetProperty(1.0f, 32, 32);
	StaticSphere.Init();

	ScreenQuad.SetMaterial(MaterialContainer.data()[0]);
	ScreenQuad.Init();

	D3DHelper::AllocConstantBuffer(Device, &StaticSphere);
	D3DHelper::AllocConstantBuffer(Device, &ScreenQuad);
	
	D3DHelper::AllocDebugLineBuffer(Device, &StaticSphere, 0);
	D3DHelper::AllocDebugLineBuffer(Device, &StaticSphere, 1);

	// Initialize for OMSetRenderTargets method.
	for (UINT i = 0; i < BUFFERCOUNT; i++)
		SettingRenderTargets[i] = GBuffer[i].RTV;

	ImGuiVar.Radio_BufferVisualization = 0;
	ImGuiVar.Radio_Technique = 0;

}

/** ���� ���� **/
// 1. ��ũ�� Ŭ���� (����Ÿ��, ����)
// 2. G���ۿ� ����
// 3. G���ۿ� ������ �̹��� (�ؽ��� ����)�� SRV�� �Ѿ��.
// 4. SRV�� ����Ʈ�� �ѱ�� ����Ʈ���� �Ѿ ���� ������ (������, ���)�� �̿��� ������ ������ �����Ѵ�.
// 5. �ȼ� ���̴� ��� �������� ��ũ�� ������ �ؽ��ķ� ���ø��ȴ�.
// 5. ��ũ�� ���尡 ������ ���չ��۸� ����Ѵ�. 

void RenderManager::OnUpdate()
{
	ClearScreen(DirectX::Colors::Green);

}

void RenderManager::OnRender()
{
	DrawObject(&StaticSphere);
	DrawDebugLine(&StaticSphere,0);
//	DrawDebugLine(&StaticSphere,1);
	//DrawDebugLine(&StaticSphere,2);


	ScreenQuad.SetPosition(GBuffer[0].SRV);
	ScreenQuad.SetNormal(GBuffer[1].SRV);
	ScreenQuad.SetBinormal(GBuffer[2].SRV);
	ScreenQuad.SetTangent(GBuffer[3].SRV);
	ScreenQuad.SetColor(GBuffer[4].SRV);
	ScreenQuad.SetDepth(GBuffer[BUFFERCOUNT - 1].SRV);


	ScreenQuad.SetPixelOffset(&GBufferVar.ViewportDimensions);
	DrawObject(&ScreenQuad,(TECHNIQUES)(ImGuiVar.Radio_Technique), (RENDERBUFFER)(ImGuiVar.Radio_BufferVisualization));


	RenderImGui();

}

void RenderManager::OnRelease()
{
	StaticSphere.Release();
	ScreenQuad.Release();

	MergeBuffer->Release();
	DepthStencilView->Release();
	D3DHelper::ReleaseGBuffer(GBuffer, DepthStencilView);

}

void RenderManager::RenderImGui()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		ImGui::Begin("Menu");
		ImGui::Text("Buffer Visualization");
		ImGui::BeginGroup();
		ImGui::RadioButton("Geometry", &ImGuiVar.Radio_Technique, 0); ImGui::SameLine();
		ImGui::RadioButton("Buffer", &ImGuiVar.Radio_Technique, 1);

		switch(ImGuiVar.Radio_Technique)
		{
		case 0:
			ImGuiVar.Radio_BufferVisualization = 1;
			break;
		case 1:
			ImGui::RadioButton("Normal", &ImGuiVar.Radio_BufferVisualization, 0); ImGui::SameLine();
			ImGui::RadioButton("Binormal", &ImGuiVar.Radio_BufferVisualization, 1); ImGui::SameLine();
			ImGui::RadioButton("Tangent", &ImGuiVar.Radio_BufferVisualization, 2);
			ImGui::RadioButton("Depth", &ImGuiVar.Radio_BufferVisualization, 3);
			break;
		}

		ImGui::EndGroup();
		ImGui::End();
	}


	ImGui::EndFrame();
	ImGui::Render();
	Context->OMSetRenderTargets(1, &MergeBuffer, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void RenderManager::InitImGui()
{
	ImGuiVar.Radio_BufferVisualization = 0;
}
