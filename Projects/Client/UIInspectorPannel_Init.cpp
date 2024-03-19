#include "pch.h"
#include "UIInspectorPannel.h"

#include "UITransform.h"
#include "UIMeshRender.h"
#include "UICollider2D.h"
#include "UILight2D.h"
#include "UIAnimator2D.h"
#include "UICamera.h"
#include "UITileMap.h"
#include "UIParticleSystem.h"
#include "UIPrefab.h"

void UIInspectorPannel::CreateComponentUI()
{
	m_arrCompUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new UITransform;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrCompUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new UICollider2D;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	//COLLIDER3D,

	m_arrCompUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new UIAnimator2D;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	//ANIMATOR3D,

	m_arrCompUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new UILight2D;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	//LIGHT3D,	// 3차원 광원

	//CAMERA,		// 카메라 기능
	m_arrCompUI[(UINT)COMPONENT_TYPE::CAMERA] = new UICamera;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrCompUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new UIMeshRender;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrCompUI[(UINT)COMPONENT_TYPE::TILEMAP] = new UITileMap;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrCompUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new UIParticleSystem;
	AddChild(m_arrCompUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

	//SKYBOX,
	//DECAL,
	//LANDSCAPE,
	//SCRIPT,

	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCompUI[i])
		{
			m_arrCompUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetInspector(this);
			m_arrCompUI[i]->Deactivate();
		}
	}
};


#include "UIMeshData.h"
#include "UIMesh.h"
#include "UIMaterial.h"
#include "UIGraphicsShader.h"
#include "UIComputeShader.h"
#include "UITexture.h"
#include "UISound.h"
#include "UIFSM.h"

void UIInspectorPannel::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new UIMesh;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::MESH]);

	m_arrAssetUI[(UINT)ASSET_TYPE::MESHDATA] = new UIMeshData;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::MESHDATA]);

	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new UIMaterial;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL]);

	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHICS_SHADER] = new UIGraphicsShader;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHICS_SHADER]);

	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new UIComputeShader;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER]);

	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new UITexture;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE]);

	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new UISound;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::SOUND]);

	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new UIPrefab;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB]);

	m_arrAssetUI[(UINT)ASSET_TYPE::FSM] = new UIFSM;
	AddChild(m_arrAssetUI[(UINT)ASSET_TYPE::FSM]);

	for (int i = 0; i < (int)ASSET_TYPE::END; ++i)
	{
		if (m_arrAssetUI[i])
			m_arrAssetUI[i]->Deactivate();
	}
}
;

