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
	TRANSFORM,	// ������Ʈ�� ��ġ,ũ��,ȸ��
	COLLIDER2D,	// 2���� �浹ü
	COLLIDER3D, // 3���� �浹ü
	ANIMATOR2D,	// ��������Ʈ Animation
	ANIMATOR3D,	// Bone Skinning Animation
	LIGHT2D,	// 2���� ����
	LIGHT3D,	// 3���� ����
	CAMERA,		// ī�޶� ���
	STATEMACHINE, // ���¸ӽ�
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
	LESS,				// �� ������ ���			, ���� ��� O
	LESS_EQUAL,			// �� �����ų� ������ ���	, ���� ��� O

	GREATER,			// �� �ָ� ���				, ���� ��� O
	GREATER_EQAUL,		// �� �ְų� ������ ���	, ���� ��� O

	NO_TEST,			// ���� �׽�Ʈ X			, ���� ��� O
	NO_WRITE,			// ���� �׽�Ʈ O			, ���� ��� X
	NO_TEST_NO_WRITE,	// ���� �׽�Ʈ X			, ���� ��� X

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
	DOMAIN_OPAQUE,		// ������ ������
	DOMAIN_MASKED,		// ���� �߶󳻱�
	DOMAIN_TRANSPARENT,	// ������
	DOMAIN_POSTPROCESS,	// ��ó��

	DOMAIN_DEBUG
};

enum class PARTICLE_MODULE
{
	SPAWN,				// ��ƼŬ ����
	DRAG,				// �̵� ���
	SCALE,				// ũ�� ��ȭ
	ADD_VELOCITY,		// �ʱ� �ӵ� ����
	NOISE_FORCE,		// ��ġ ������
	CACULATE_FORCE,		// �� ���

	RENDER,				// ������ �ɼ�
	PARTICLE_COLISION,	// ��ƼŬ �浹

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