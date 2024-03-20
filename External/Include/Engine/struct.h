#pragma once

// ����
// - ������ ��ǥ �� �پ��� ������ ������ ���� �� ����
// - Normal Vector ��������, Tangent Vector ��������, BiNormal Vector ...
struct Vtx
{
	Vec3 vPos;		// ���� ��ǥ
	Vec4 vColor;	// ���� ����
	Vec2 vUV;		// UV ��ǥ�� or Texture Coodinate
};

struct FDebugShapeInfo
{
	DEBUG_SHAPE	ShapeType;
	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRot;
	Matrix		matWorld;

	Vec3		vColor;
	float		fLifeTime;
	float		fDuration;
	bool		bDepthTest;
};

struct FLightInfo
{
	int		LightType;	// ���� Ÿ��
	Vec4	vColor;		// ���� ����		(������ ���� ����)
	Vec4	vSpecular;	// �ݻ籤			(������ ��ü ǥ�鿡 �ݻ�Ǵ� ����)
	Vec4	vAmbient;	// ȯ�汤(�ֺ���)	(������ ���� ����Ǵ� �ּ����� ��)

	Vec3	vWorldPos;
	Vec3	vWorldDir;
	float	fRadius;
	float	fAngle;

	Vec3	vPadding;
};

struct FPixel
{
	BYTE r, g, b, a;
};

// FParticle: ���� �ϳ��� ������ ������ ���� ����ü
struct FParticle
{
	Vec4	vLocalPos;
	Vec4	vWorldPos;
	Vec4	vWorldInitScale;	// �ʱ� ũ��
	Vec4	vWorldScale;		// ���� ũ��
	Vec4	vWorldRotation;
	Vec3	vVelocity;
	Vec4	vColor;
	Vec4	vForce;				// ���ڿ� ������ �� �ѷ�

	Vec3	vNosieForce;		// NosieForce ���� ���� ���� ��
	float	NosieForceTime;		// NosieForce ���ù��� �ð�

	float	Mass;
	float	Life;				// �� ����
	float	Age;				// ���� ����
	float	NormalizedAge;		// Life �������� ����ȭ�� Age
	int		Active;

	// ----------------------------------- particle collision
	//Vec4	vNewLocalPos;
	//Vec3	vNewVelocity;
	//float	fTemperature;		// ���ڰ� �ֱٿ� �浹�ߴ��� ����
	//int		iNumCollisions;

	//Vec3	Padding;
};

// FParticleModule: Particle System�� ������ ���� ����ü
struct FParticleModule
{
	// Module: Spawn | ��ƼŬ ����
	Vec4	vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	float	LifeMin;
	float	LifeMax;
	float	MassMin;
	float	MassMax;
	int		SpawnRate;
	int		SpaceType;			// ��ǥ�� (0: Local, 1: World)

	int		SpawnShape;			// (0: Sphere, 1: Box)
	float	Radius;				// Spawn Shape - Sphere : ������ ����
	Vec4	vSpawnBoxScale;		// Spawn Shape - Box : Box ũ��

	// Module: Drag	| �̵� ���
	float	DragTime;

	// Module: Scale | ũ�� ��ȭ
	Vec4	vScaleRatio;

	// Module: Add Velocity	| �ʱ� �ӵ� ����
	int		AddVelocityType;	// 0: from center, 1: to center, 2: fix direction
	float	SpeedMin;
	float	SpeedMax;
	float	FixedAngle;
	Vec4	FixedDirection;

	// Module: Noise Force | ��ġ ������
	float	NosieForceScale;
	float	NosieForceTerm;

	// Module: Cacluate Force | �� ���
	int		Gravity;			// 0: Off, 1: On
	Vec3	GravityScale;
	
	// Module: Render | ������ �ɼ�
	int		VelocityAlignment;	// 0: Off, 1: On 
	int		AlphaBasedLife;		// 0: Off, 1: NormalizedAge, 2: Age
	float	AlphaMaxAge;

	// Module: Collision | Particle Collision
	
	// Module On / Off
	int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
};

// FSpawnCount: Particle Spawn ����ȭ�� ���
struct FSpawnCount
{
	int SpawnCount;
	int iPadding[3];
};

//---------------------------------
// Constant Buffer ���� ����ü
//---------------------------------
// - constant buffer�� ������ ���� �ݵ�� 16Byte ũ�� ���
struct FTransform
{
	Matrix matWorld;
	Matrix matWorldInv;

	Matrix matView;
	Matrix matViewInv;

	Matrix matProj;
	Matrix matProjInv;

	Matrix matWV;	// World * View
	Matrix matWVP;	// World * View * Proj
};
extern FTransform g_Transform;	// �ܺ� ���� (���� ����)

// tMatrlConst: material���� ����� params ��Ƶ� ����ü
// SCALAR_PARAM: define const paramater type that can be transffered to mateiral
struct FMatConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	// texture binding ���� ����
	int bTex[(UINT)TEX_PARAM::END];

	int iPadding[2];
};

struct FAnimData2D
{
	Vec2 vLeftTop;
	Vec2 vCutSize;
	Vec2 vBackgroundSize;
	Vec2 vOffset;
	Vec2 g_vAtlasSize;
	int	 UseAnim2D;
	float vPadding;
};

struct FGlobalData		// ���� ����ϴ� ������ ����
{
	Vec2	g_RenderResolution;
	float	g_dt;
	float	g_time;
	int		g_Light2DCount;
	int		g_Light3DCount;
	Vec2	g_vPadding;
};
extern FGlobalData g_Global;