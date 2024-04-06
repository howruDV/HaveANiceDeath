#include "pch.h"
#include "UIMeshRender.h"
#include "UIInspectorPannel.h"
#include "UIListPannel.h"
#include "UIMaterial.h"
#include "CImGuiMgr.h"
#include "func_ImGUI.h"

#include <Engine/CMeshRender.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CMesh.h>

UIMeshRender::UIMeshRender()
	: UIComponent("MeshRender", "##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
}

UIMeshRender::~UIMeshRender()
{
}

void UIMeshRender::render_update()
{
	if (not GetTargetObject())
		return;

	CGameObject* pTarget = GetTargetObject();
	CMeshRender* pMeshRender = pTarget->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMat = pMeshRender->GetMaterial();
	string meshname, matname;
	if (pMesh.Get()) meshname = WstrToStr(pMesh->GetKey()).c_str();
	if (pMat.Get()) matname = WstrToStr(pMat->GetKey()).c_str();

	TextBox("Mesh    "); ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)meshname.c_str(), meshname.length(), ImGuiInputTextFlags_ReadOnly);
	
	// Mesh Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (pAsset->GetType() == ASSET_TYPE::MESH)
				GetTargetObject()->MeshRender()->SetMesh((CMesh*)pAsset);
		}

		ImGui::EndDragDropTarget();
	}
	
	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(20, 20)))
	{
		UIListPannel* pList = (UIListPannel*)CImGuiMgr::GetInst()->FindUI("##List");
		vector<string> vecMeshName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MESH, vecMeshName);

		pList->SetModal(true);
		pList->AddString(vecMeshName);
		//pList->SetDBClickCallback(::MeshSelect);
		pList->SetDBClickeDelegate(this, (DELEGATE_1)&UIMeshRender::MeshSelect);
		pList->Activate();
	}

	TextBox("Material"); ImGui::SameLine();
	ImGui::InputText("##MatName", (char*)matname.c_str(), matname.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	// Material Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (pAsset->GetType() == ASSET_TYPE::MATERIAL)
				GetTargetObject()->MeshRender()->SetMaterial((CMaterial*)pAsset); // @TODO 합쳐도 되지않나?
		}

		ImGui::EndDragDropTarget();
	}

	if (ImGui::Button("##MatBtn", ImVec2(20, 20)))
	{
		UIListPannel* pList = (UIListPannel*)CImGuiMgr::GetInst()->FindUI("##List");
		vector<string> vecMatName;
		CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::MATERIAL, vecMatName);

		pList->SetModal(true);
		pList->AddString(vecMatName);
		pList->SetDBClickeDelegate(this, (DELEGATE_1)&UIMeshRender::MaterialSelect);
		pList->Activate();
	}

	// Material 인자 표시
	if(pMat.Get())
		UIMaterial::render_UIMatParam(pMat, pMat->GetShader());
}

void UIMeshRender::MeshSelect(DWORD_PTR _ptr)
{
	wstring strMeshName = StrToWstr((char*)_ptr);
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshName);

	GetInspector()->GetTargetObject()->MeshRender()->SetMesh(pMesh);
}

void UIMeshRender::MaterialSelect(DWORD_PTR _ptr)
{
	wstring strMatName = StrToWstr((char*)_ptr);
	Ptr<CMaterial> pMat = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMatName);

	GetInspector()->GetTargetObject()->MeshRender()->SetMaterial(pMat);
}

void MeshSelect(DWORD_PTR _ptr)
{
	wstring strMeshName = StrToWstr((char*)_ptr);
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshName);
	
	CGameObject* pTarget = ((UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector"))->GetTargetObject();
	pTarget->MeshRender()->SetMesh(pMesh);
}

void MaterialSelect(DWORD_PTR _ptr)
{
	wstring strMatName = StrToWstr((char*)_ptr);
	Ptr<CMaterial> pMat = CAssetMgr::GetInst()->FindAsset<CMaterial>(strMatName);

	CGameObject* pTarget = ((UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector"))->GetTargetObject();
	pTarget->MeshRender()->SetMaterial(pMat);
}