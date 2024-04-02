#pragma once

#define SINGLE(classType)	private:\
								classType();\
								~classType();\
							friend class CSingleton<classType>;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define DT	CTimeMgr::GetInst()->GetDeltaTime()
#define DT_ENGINE	CTimeMgr::GetInst()->GetEngineDeltaTime()
#define DTd_ENGINE	CTimeMgr::GetInst()->GetEngineDeltaTime_d()

#define KEY_CHECK(Key, State) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::State
#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define LAYER_MAX 32

#define CLONE(TYPE) virtual TYPE* Clone() { return new TYPE(*this); }
#define CLONE_DISABLE(TYPE) TYPE* Clone() { return nullptr; }\
							TYPE(const TYPE& _OriginBuffer) = delete;

// Direction Vector
enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT
};

enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	PREFAB,
	TEXTURE,
	MATERIAL,
	SOUND,
	COMPUTE_SHADER,
	GRAPHICS_SHADER,
	FSM,

	END,
};
extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];

enum class COMPONENT_TYPE
{
	TRANSFORM,	// 오브젝트의 위치,크기,회전
	COLLIDER2D,	// 2차원 충돌체
	COLLIDER3D, // 3차원 충돌체
	ANIMATOR2D,	// 스프라이트 Animation
	ANIMATOR3D,	// Bone Skinning Animation
	LIGHT2D,	// 2차원 광원
	LIGHT3D,	// 3차원 광원
	CAMERA,		// 카메라 기능
	STATEMACHINE, // 상태머신
	MOVEMENT,

	// Render Component
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,
	// Before End: basic type
	// -------------------------------
	// After End: custom type (added by user)

	SCRIPT,
};
extern const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END];

// Constant Buffer Type
enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	ANIM2D_DATA,
	GLOBAL_DATA,
	ANIMTATION,

	END,
};

// Rasterizer State Type
enum class RS_TYPE
{
	CULL_BACK,	// CCW(Counter Clock Wise) - default
	CULL_FRONT,	// CW
	CULL_NONE,
	WIRE_FRAME,	// outline

	END,
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,				// 더 가까우면 통과			, 깊이 기록 O
	LESS_EQUAL,			// 더 가깝거나 같으면 통과	, 깊이 기록 O

	GREATER,			// 더 멀면 통과				, 깊이 기록 O
	GREATER_EQAUL,		// 더 멀거나 같으면 통과	, 깊이 기록 O

	NO_TEST,			// 깊이 테스트 X			, 깊이 기록 O
	NO_WRITE,			// 깊이 테스트 O			, 깊이 기록 X
	NO_TEST_NO_WRITE,	// 깊이 테스트 X			, 깊이 기록 X

	END,
};

// Blend State Type
enum class BS_TYPE
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_ONE,

	END,
};


// Sampler Type
enum class SAMPLER_TYPE
{
	ANIS,
	MIPMAP,

	END,
};

// SCALAR_PARAM: define const paramater type that can be transffered to mateiral 
enum class SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

enum class SCRIPT_PARAM
{
	BOOL,
	INT,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	OBJECT,
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,

	CUBE,
	SPHERE,

	CROSS,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 무조건 불투명
	DOMAIN_MASKED,		// 투명 잘라내기
	DOMAIN_TRANSPARENT,	// 반투명
	DOMAIN_POSTPROCESS,	// 후처리

	DOMAIN_DEBUG
};

enum class PARTICLE_MODULE
{
	SPAWN,				// 파티클 생성
	DRAG,				// 이동 경로
	SCALE,				// 크기 변화
	ADD_VELOCITY,		// 초기 속도 지정
	NOISE_FORCE,		// 위치 노이즈
	CACULATE_FORCE,		// 힘 계산

	RENDER,				// 렌더링 옵션
	PARTICLE_COLISION,	// 파티클 충돌

	END,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
	NONE,
};

enum class UNIT_DIRX
{
	LEFT,
	RIGHT,
};
enum class UNIT_DIRY
{
	UP,
	DOWN,
};