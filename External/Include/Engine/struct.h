#pragma once

// 정점
// - 정점은 좌표 외 다양한 정보를 가지고 있을 수 있음
// - Normal Vector 법선벡터, Tangent Vector 접선벡터, BiNormal Vector ...
struct Vtx
{
	Vec3 vPos;		// 정점 좌표
	Vec4 vColor;	// 색상 정보
	Vec2 vUV;		// UV 좌표계 or Texture Coodinate
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
	int		LightType;	// 광원 타입
	Vec4	vColor;		// 빛의 색상		(광원의 순수 색상)
	Vec4	vSpecular;	// 반사광			(광원이 물체 표면에 반사되는 색상)
	Vec4	vAmbient;	// 환경광(주변광)	(광원에 의해 보장되는 최소한의 빛)

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

// FParticle: 입자 하나가 가지는 정보를 담은 구조체
struct FParticle
{
	Vec4	vLocalPos;
	Vec4	vWorldPos;
	Vec4	vWorldInitScale;	// 초기 크기
	Vec4	vWorldScale;		// 현재 크기
	Vec4	vWorldRotation;
	Vec3	vVelocity;
	Vec4	vColor;
	Vec4	vForce;				// 입자에 누적된 힘 총량

	Vec3	vNosieForce;		// NosieForce 모듈로 인한 랜덤 힘
	float	NosieForceTime;		// NosieForce 세팅받은 시간

	float	Mass;
	float	Life;				// 총 수명
	float	Age;				// 현재 수명
	float	NormalizedAge;		// Life 기준으로 정규화된 Age
	int		Active;

	// ----------------------------------- particle collision
	//Vec4	vNewLocalPos;
	//Vec3	vNewVelocity;
	//float	fTemperature;		// 입자가 최근에 충돌했는지 여부
	//int		iNumCollisions;

	//Vec3	Padding;
};

// FParticleModule: Particle System의 세팅을 담은 구조체
struct FParticleModule
{
	// Module: Spawn | 파티클 생성
	Vec4	vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	float	LifeMin;
	float	LifeMax;
	float	MassMin;
	float	MassMax;
	int		SpawnRate;
	int		SpaceType;			// 좌표계 (0: Local, 1: World)

	int		SpawnShape;			// (0: Sphere, 1: Box)
	float	Radius;				// Spawn Shape - Sphere : 반지름 길이
	Vec4	vSpawnBoxScale;		// Spawn Shape - Box : Box 크기

	// Module: Drag	| 이동 경로
	float	DragTime;

	// Module: Scale | 크기 변화
	Vec4	vScaleRatio;

	// Module: Add Velocity	| 초기 속도 지정
	int		AddVelocityType;	// 0: from center, 1: to center, 2: fix direction
	float	SpeedMin;
	float	SpeedMax;
	float	FixedAngle;
	Vec4	FixedDirection;

	// Module: Noise Force | 위치 노이즈
	float	NosieForceScale;
	float	NosieForceTerm;

	// Module: Cacluate Force | 힘 계산
	int		Gravity;			// 0: Off, 1: On
	Vec3	GravityScale;
	
	// Module: Render | 렌더링 옵션
	int		VelocityAlignment;	// 0: Off, 1: On 
	int		AlphaBasedLife;		// 0: Off, 1: NormalizedAge, 2: Age
	float	AlphaMaxAge;

	// Module: Collision | Particle Collision
	
	// Module On / Off
	int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
};

// FSpawnCount: Particle Spawn 동기화에 사용
struct FSpawnCount
{
	int SpawnCount;
	int iPadding[3];
};

//---------------------------------
// Constant Buffer 대응 구조체
//---------------------------------
// - constant buffer는 정렬을 위해 반드시 16Byte 크기 사용
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
extern FTransform g_Transform;	// 외부 변수 (전역 변수)

// tMatrlConst: material에서 사용할 params 모아둔 구조체
// SCALAR_PARAM: define const paramater type that can be transffered to mateiral
struct FMatConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	// texture binding 여부 전달
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

struct FGlobalData		// 자주 사용하는 정보들 전달
{
	Vec2	g_RenderResolution;
	float	g_dt;
	float	g_time;
	int		g_Light2DCount;
	int		g_Light3DCount;
	Vec2	g_vPadding;
};
extern FGlobalData g_Global;