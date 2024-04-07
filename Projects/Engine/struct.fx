#ifndef _STRUCT
#define _STRUCT
// =======================================
// struct.fx: struct type ����
// =======================================

struct tLightColor
{
    float4 vColor;      // ���� ����		(������ ���� ����)
    float4 vSpecular;   // �ݻ籤			(������ ��ü ǥ�鿡 �ݻ�Ǵ� ����)
    float4 vAmbient;    // ȯ�汤(�ֺ���)	(������ ���� ����Ǵ� �ּ����� ��)
};

struct tLightInfo
{
    int LightType;      // ���� Ÿ��
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
    float4 vWorldInitScale;     // �ʱ� ũ��
    float4 vWorldScale;         // ���� ũ��
    float4 vWorldRotation;
    float3 vVelocity;
    float4 vColor;
    float4 vForce;              // ���ڿ� ������ �� �ѷ�

    float3 vNosieForce;         // NosieForce ���� ���� ���� ��
    float NosieForceTime;       // NosieForce ���ù��� �ð�

    float Mass;
    float Life;                 // �� ����
    float Age;                  // ���� ����
    float NormalizedAge;         // Life �������� ����ȭ�� Age
    int Active;
};

struct FParticleModule
{
	// Module: Spawn | ��ƼŬ ����
    float4 vSpawnColor;
    float4 vSpawnScaleMin;
    float4 vSpawnScaleMax;
    float LifeMin;
    float LifeMax;
    float MassMin;
    float MassMax;
    int SpawnRate;
    int SpaceType;              // ��ǥ�� (0: Local, 1: World)

    int SpawnShape;             // (0: Sphere, 1: Box)
    float Radius;               // Spawn Shape - Sphere : ������ ����
    float4 vSpawnBoxScale;      // Spawn Shape - Box : Box ũ��

	// Module: Drag	| �̵� ���
    float DragTime;

	// Module: Scale | ũ�� ��ȭ
    float4 vScaleRatio;

	// Module: Add Velocity	| �ʱ� �ӵ� ����
    int AddVelocityType;        // 0: from center, 1: to center, 2: fix direction
    float SpeedMin;
    float SpeedMax;
    float FixedAngle;
    float4 FixedDirection;

	// Module: Noise Force | ��ġ ������
    float NosieForceScale;
    float NosieForceTerm;

	// Module: Cacluate Force | �� ���
    int Gravity;                // 0: Off, 1: On
    float3 GravityScale;
    
	// Module: Render | ������ �ɼ�
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