#include "pch.h"
#include "global.h"

// =======================================
// extern: extern ������ ��� ����
// =======================================

FTransform g_Transform = {};
FGlobalData g_Global = {};

// essty type name: Cpp�� enum name �����ϴ� ��� �����Ƿ� ��Ī�Ǵ� string ���� ����
const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]
{
	"MESH",
	"MESHDATA",
	"PREFAB",
	"TEXTURE",
	"MATERIAL",
	"SOUND",
	"COMPUTE_SHADER",
	"GRAPHICS_SHADER",
	"FSM",
};