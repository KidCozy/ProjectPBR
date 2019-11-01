#include "RenderManager.h"

void RenderManager::SetMaterialPath()
{
	Material* ArrayContainer[DEFAULT_MATERIALSIZE];

	for (UINT i = 0; i < DEFAULT_MATERIALSIZE; i++)
	{
		ArrayContainer[i] = new Material();
	}

	ArrayContainer[0]->SetFile(L"DefaultShader.fx");
	ArrayContainer[1]->SetFile(L"DefaultShader.fx");
	ArrayContainer[2]->SetFile(L"ScreenQuadShader.fx");
	ArrayContainer[3]->SetFile(L"RaytraceShader.fx");

	MaterialContainer.push_back(ArrayContainer[0]);
	MaterialContainer.push_back(ArrayContainer[1]);
	MaterialContainer.push_back(ArrayContainer[2]);
	MaterialContainer.push_back(ArrayContainer[3]);

}

void RenderManager::BindBuffer(Geometry * Geometry)
{
	static const UINT Stride = sizeof(Vertex);
	static const UINT Offset = 0;

	Context->IASetVertexBuffers(0, 1, &Geometry->GetBuffer()->VBuffer, &Stride, &Offset);
	Context->IASetIndexBuffer(Geometry->GetBuffer()->IBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void RenderManager::DrawObject(Geometry * Object,TECHNIQUES Technique, PASS RenderBuffer)
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

void RenderManager::RDragNotify(WinMessage* Event, WinMessage* NewEvent)
{
	StaticCamera.RDragNotify(Event, NewEvent);
}

void RenderManager::LDragNotify(WinMessage * Event, WinMessage * NewEvent)
{
}

void RenderManager::KeyEnterNotify(WinMessage * Event, WinMessage * NewEvent)
{
	StaticCamera.KeyEnterNotify(Event, NewEvent);
}

void RenderManager::KeyPressNotify(WinMessage * Event, WinMessage * NewEvent)
{
}

void RenderManager::KeyReleaseNotify(WinMessage * Event, WinMessage * NewEvent)
{
}

void RenderManager::PostInitialize()
{
	SetMaterialPath();
	D3DHelper::CompileShader(Device, MaterialContainer.data()[0]);
	D3DHelper::CompileShader(Device, MaterialContainer.data()[1]);
	D3DHelper::CompileShader(Device, MaterialContainer.data()[3]);

}

void RenderManager::ClearScreen(XMVECTORF32 ClearColor)
{
	Context->ClearRenderTargetView(MergeBuffer, ClearColor);
	Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (UINT i = 0; i < (BUFFERCOUNT-2); i++)
		Context->ClearRenderTargetView(GBuffer[i].RTV, ClearColor);


}

void RenderManager::OnInit()
{
	Skybox.SetMaterial(MaterialContainer.data()[1]);
	StaticSphere.SetMaterial(MaterialContainer.data()[0]);

	StaticCube.SetMaterial(MaterialContainer.data()[3]);
	StaticCube.Init();
	StaticCube.SetColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	D3DHelper::AllocConstantBuffer(Device, &StaticCube);

	Skybox.SetProperty(20.0f, 64, 64);
	Skybox.Init();

	Skybox.AddEnvironmentTexture(Device, L"Resources/Textures/Skybox_crowd.dds");
	StaticSphere.AddEnvironmentTexture(Device, L"Resources/Textures/Skybox_beach.dds");
	StaticSphere.SetProperty(1.0f, 256, 256);
	StaticSphere.Init();
	StaticSphere.SetColor(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	ScreenQuad.SetMaterial(MaterialContainer.data()[3]);
	ScreenQuad.Init();

	D3DHelper::AllocConstantBuffer(Device, &StaticSphere);
	D3DHelper::AllocConstantBuffer(Device, &ScreenQuad);
	D3DHelper::AllocConstantBuffer(Device, &Skybox);
	
	D3DHelper::AllocDebugLineBuffer(Device, &StaticSphere, 0);
	D3DHelper::AllocDebugLineBuffer(Device, &StaticSphere, 1);

	// Initialize for OMSetRenderTargets method.
	for (UINT i = 0; i < BUFFERCOUNT; i++)
		SettingRenderTargets[i] = GBuffer[i].RTV;

	ImGuiVar.Radio_BufferVisualization = 0;
	ImGuiVar.Radio_Technique = 0;
	ImGuiVar.Radio_DebugLine = false;
	ImGuiVar.Radio_Spd = false;

	v = Skybox.GetMaterial()->GetTextures()["Skybox_crowd.dds"]->GetShaderResourceView();
	sr = Skybox.GetMaterial()->GetShaderVariables()["CubeSlot"]->AsShaderResource();




}

/** 렌더 순서 **/
// 1. 스크린 클리어 (렌더타겟, 뎁스)
// 2. G버퍼에 렌더
// 3. G버퍼에 렌더된 이미지 (텍스쳐 정보)가 SRV로 넘어간다.
// 4. SRV를 이펙트에 넘기고 이펙트에서 넘어간 버퍼 정보들 (포지션, 노멀)을 이용해 라이팅 연산을 수행한다.
// 5. 픽셀 쉐이더 출력 정보들이 스크린 쿼드의 텍스쳐로 샘플링된다.
// 5. 스크린 쿼드가 렌더된 병합버퍼를 출력한다. 

void RenderManager::OnUpdate()
{
	ClearScreen(DirectX::Colors::Green);

	XMMATRIX SpherePos = XMMatrixIdentity();

	SpherePos += XMMatrixTranslation(0.0f, 0.0f, 5.0f);


	StaticSphere.GetMaterial()->SetWorldMatrix(SpherePos);
}	

void RenderManager::OnRender()
{

	StaticSphere.GetMaterial()->GetShaderVariables()["ViewDirection"]->AsVector()->SetFloatVector(StaticCamera.GetDirection().m128_f32);
//	StaticSphere.GetMaterial()->GetShaderVariables()["ViewPosition"]->AsVector()->SetFloatVector(StaticCamera.GetTransform().GetPosition().m128_f32);
	//StaticSphere.GetTransform()->Rotate(XMVectorSet(0.1f, 0.0f, 0.0f,1.0f));
	
	StaticCube.GetMaterial()->GetShaderVariables()["ViewDirection"]->AsVector()->SetFloatVector(StaticCamera.GetDirection().m128_f32);
	StaticCube.GetMaterial()->GetShaderVariables()["ViewPosition"]->AsVector()->SetFloatVector(StaticCamera.GetTransform().GetPosition().m128_f32);


	DrawObject(&StaticSphere);
	DrawObject(&StaticCube, (TECHNIQUES)TECHNIQUE_RAY, (PASS)0);

	if(ImGuiVar.Radio_DebugLine)
		DrawDebugLine(&StaticSphere,0);
	//DrawObject(&Skybox);

	

	if (FAILED(sr->SetResource(Skybox.GetMaterial()->GetTextures()["Skybox_crowd.dds"]->GetShaderResourceView())))
	{
		MessageBox(NULL, L"Failed to set cubeslot resource.", 0, 0);
	}


	ScreenQuad.SetPosition(GBuffer[0].SRV);
	ScreenQuad.SetNormal(GBuffer[1].SRV);
	ScreenQuad.SetBinormal(GBuffer[2].SRV);
	ScreenQuad.SetTangent(GBuffer[3].SRV);
	ScreenQuad.SetColor(GBuffer[4].SRV);
	ScreenQuad.SetAlbedo(GBuffer[5].SRV);
	ScreenQuad.SetRay(GBuffer[BUFFERCOUNT - 2].SRV);
	ScreenQuad.SetDepth(GBuffer[BUFFERCOUNT - 1].SRV);
	

	ScreenQuad.SetPixelOffset(&GBufferVar.ViewportDimensions);
	DrawObject(&ScreenQuad,(TECHNIQUES)2, (PASS)0);
	

	RenderImGui();

}

void RenderManager::OnRelease()
{
	StaticSphere.Release();
	ScreenQuad.Release();

	MergeBuffer->Release();
	DepthStencilView->Release();
	D3DHelper::ReleaseGBuffer(GBuffer, DepthStencilView);

	HistoSample.SampleTex->Release();
	HistoSample.Data = nullptr;

}

void RenderManager::RenderImGui()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	{
		ID3DX11EffectScalarVariable* Rough = StaticSphere.GetMaterial()->GetShaderVariables()["Roughness"]->AsScalar();
		static float Roughness;
		static bool RenderEnvMap = false;


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
		ImGui::Checkbox("Normal Debug Line", &ImGuiVar.Radio_DebugLine);
		ImGui::Checkbox("Spectral Power Distribution", &ImGuiVar.Radio_Spd); ImGui::SameLine();
		ImGui::Checkbox("Render Environment Map", &RenderEnvMap);
		ImGui::SliderFloat("Roughness", &Roughness, 0.01f,1.0f);
		Rough->SetFloat(Roughness);

		ImGui::EndGroup();
		ImGui::End();


		//if (RenderEnvMap)
		//{
		//	StaticSphere.GetMaterial()->GetShaderVariables()["CubeSlot"]->AsShaderResource()->SetResource(StaticSphere.GetMaterial()->GetTextures()["Skybox_beach.dds"]->GetShaderResourceView());

		//}
		//else
		//{
		//	StaticSphere.GetMaterial()->GetShaderVariables()["CubeSlot"]->AsShaderResource()->SetResource(nullptr);

		//}

	}

	if (ImGuiVar.Radio_Spd)
	{
		static float c[] = { 0.0f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,4.0,7.4f,1.2f,3.7f,9.3f };
		static int t = 60;



		HistoSample.SampleDesc.ArraySize = 1;
		HistoSample.SampleDesc.BindFlags = NULL;
		HistoSample.SampleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		HistoSample.SampleDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		HistoSample.SampleDesc.Height = Height;
		HistoSample.SampleDesc.Width = Width;
		HistoSample.SampleDesc.MipLevels = 1;
		HistoSample.SampleDesc.MiscFlags = 0;
		HistoSample.SampleDesc.Usage = D3D11_USAGE_STAGING;
		HistoSample.SampleDesc.SampleDesc.Count = 1;
		HistoSample.SampleDesc.SampleDesc.Quality = 0;

		Device->CreateTexture2D(&HistoSample.SampleDesc, nullptr, &HistoSample.SampleTex);

		Context->CopyResource(HistoSample.SampleTex, GBuffer[0].Texture);
		//GBuffer[0].RTV->GetResource((ID3D11Resource**)&SampleTex);
		
		HRESULT hr;
		hr = Context->Map(HistoSample.SampleTex, 0, D3D11_MAP_READ, NULL, &HistoSample.MapResource);
		if(FAILED(hr))
		{
			MessageBox(NULL, L"Failed to mapping", 0, 0);
		}

		static int size = Width * Height * 3 * sizeof(byte);
		HistoSample.Data = new byte[size];

		float* h = new float[size];

		memcpy(HistoSample.Data, HistoSample.MapResource.pData, size);
		
		for (int i = 0; i < size; i++)
			h[i] = HistoSample.Data[i];


		ImGui::Begin("SPD", &ImGuiVar.Radio_Spd);
		{
			ImGui::NewLine();
			ImGui::PlotLines("Spectral Power Distribution", h, size, 0, "SPD",-1.0f,256, ImVec2(0, 160));
	
		}
		ImGui::End();

		delete HistoSample.Data;
		HistoSample.SampleTex->Release();

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
