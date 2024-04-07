#ifndef _STRUCT
#define _STRUCT
// =======================================
// struct.fx: struct type 정의
// =======================================

struct tLightColor
{
    float4 vColor;      // 빛의 색상		(광원의 순수 색상)
    float4 vSpecular;   // 반사광			(광원이 물체 표면에 반사되는 색상)
    float4 vAmbient;    // 환경광(주변광)	(광원에 의해 보장되는 최소한의 빛)
};

struct tLightInfo
{
    int LightType;      // 광원 타입
    tLightColor Color;

    float3 vWorldPos;
    float3 vWorldDir;
    float fRadius;
    float fAngle;

    float3 vPadding;
};

struct FTileInfo
{
    float2  vLeftTopUV;
    int     bRender;
    int     padding;
};


struct FParticle
{
    float4 vLocalPos;
    float4 vWorldPos;
    float4 vWorldInitScale;     // 초기 크기
    float4 vWorldScale;         // 현재 크기
    float4 vWorldRotation;
    float3 vVelocity;
    float4 vColor;
    float4 vForce;              // 입자에 누적된 힘 총량

    float3 vNosieForce;         // NosieForce 모듈로 인한 랜덤 힘
    float NosieForceTime;       // NosieForce 세팅받은 시간

    float Mass;
    float Life;                 // 총 수명
    float Age;                  // 현재 수명
    float NormalizedAge;         // Life 기준으로 정규화된 Age
    int Active;
};

struct FParticleModule
{
	// Module: Spawn | 파티클 생성
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    float LifeMin;
    float LifeMax;
    float MassMin;
    float MassMax;
    int SpawnRate;
    int SpaceType;              // 좌표계 (0: Local, 1: World)

    int SpawnShape;             // (0: Sphere, 1: Box)
    float Radius;               // Spawn Shape - Sphere : 반지름 길이
    float4 vSpawnBoxScale;      // Spawn Shape - Box : Box 크기

	// Module: Drag	| 이동 경로
    float DragTime;

	// Module: Scale | 크기 변화
    float4 vScaleRatio;

	// Module: Add Velocity	| 초기 속도 지정
    int AddVelocityType;        // 0: from center, 1: to center, 2: fix direction
    float SpeedMin;
    float SpeedMax;
    float FixedAngle;
    float4 FixedDirection;

	// Module: Noise Force | 위치 노이즈
    float NosieForceScale;
    float NosieForceTerm;

	// Module: Cacluate Force | 힘 계산
    int Gravity;                // 0: Off, 1: On
    float3 GravityScale;
    
	// Module: Render | 렌더링 옵션
    int VelocityAlignment;      // 0: Off, 1: On
    int AlphaBasedLife;         // 0: Off, 1: NormalizedAge, 2: Age
    float AlphaMaxAge;
    
	// Module: Collision | Particle Collision
    
	// Module On / Off
    int arrModuleCheck[8];
    float2 padding;
};

struct FSpawnCount
{
    int SpawnCount;
    int3 iPadding;
};


#endif