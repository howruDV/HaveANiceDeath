#include "pch.h"
#include "global.h"

// =======================================
// extern: extern 변수를 모아 관리
// =======================================

FTransform g_Transform = {};
FGlobalData g_Global = {};

// essty type name: Cpp은 enum name 추출하는 기능 없으므로 매칭되는 string 따로 선언
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

const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END]
{
	"TRANSFORM",
	"COLLIDER2D",
	"COLLIDER3D",
	"ANIMATOR2D",
	"ANIMATOR3D",
	"LIGHT2D",
	"LIGHT3D",
	"CAMERA",
	"STATEMACHINE",
	"MOVEMENT",
	"MESHRENDER",
	"TILEMAP",
	"PARTICLESYSTEM",
	"SKYBOX",
	"DECAL",
	"LANDSCAPE",
};