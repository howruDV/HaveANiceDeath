#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<FParticleModule>   g_ParticleModule : register(t20);
RWStructuredBuffer<FParticle>       g_ParticleBuffer : register(u0);
RWStructuredBuffer<FSpawnCount>     g_SpawnCount : register(u1);

#define MAX_COUNT   g_int_0
#define CENTER_POS  g_vec4_0.xyz
#define SPAWN_COUNT g_SpawnCount[0].SpawnCount
#define PARTICLE    g_ParticleBuffer[id.x]
#define MODULE      g_ParticleModule[0]

[numthreads(32, 1, 1)]
void CS_UpdateParticle(uint3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= (int) id.x)
        return;
    
    // case: Not Active Yet - Activation
    if (!PARTICLE.Active)
    {
        // -------------------------
        // Module : Spawn
        // -------------------------
        if (MODULE.arrModuleCheck[0] == 0)
            return;
        
        while (SPAWN_COUNT > 0)
        {
            int AliveCount = SPAWN_COUNT;
            int Exchange = SPAWN_COUNT - 1;
         
            InterlockedCompareExchange(SPAWN_COUNT, AliveCount, Exchange, Exchange); // 스레드가 진입한 순서대로 돌아가지 않는 상황 고려해, 인자1이 예상한 값(인자2)과 같을 때만 함수 실행
            // InterlockedExchange(SPAWN_COUNT, Exchange, Exchange);    // 스레드가 진입한 순서대로 돌아가지 않을 수 있어 가끔 오류 발생
         
            if (Exchange == AliveCount)
            {
                PARTICLE.Active = 1;
                PARTICLE.vNosieForce = (float3) 0.f;
                PARTICLE.NosieForceTime = 0.f;
             
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                vUV.x += g_time * 0.2f;
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;

                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0.f);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0.f);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0.f);
             
                // spawn shape - sphere
                if (MODULE.SpawnShape == 0)
                {
                    float RandomRadius = vRand.x * MODULE.Radius;
                    float RandomAngle = vRand.y * 2 * PI;
                 
                     // space 타입에 따라 생성방식 변함 (0: localspace, 1: worldspace)
                     PARTICLE.vLocalPos.xyz = float3(cos(RandomAngle), sin(RandomAngle), 0.f) * RandomRadius;
                }
                // spawn shape - box
                else
                {
                     PARTICLE.vLocalPos.x = vRand.x * MODULE.vSpawnBoxScale.x - (MODULE.vSpawnBoxScale.x / 2.f);
                     PARTICLE.vLocalPos.y = vRand.y * MODULE.vSpawnBoxScale.y - (MODULE.vSpawnBoxScale.y / 2.f);
                     PARTICLE.vLocalPos.z = 0.f;
                }
                
                PARTICLE.vWorldPos.xyz = PARTICLE.vLocalPos.xyz + CENTER_POS;
                
                PARTICLE.vWorldInitScale = PARTICLE.vWorldScale = MODULE.vSpawnScaleMin + (MODULE.vSpawnScaleMax - MODULE.vSpawnScaleMin) * vRand.z;
                PARTICLE.Age = 0.f;
                PARTICLE.Life = MODULE.LifeMin + (MODULE.LifeMax - MODULE.LifeMin) * vRand.x;
                PARTICLE.vColor = MODULE.vSpawnColor;
                PARTICLE.Mass = lerp(MODULE.MassMin, MODULE.MassMax, vRand1.x);
                
                // -------------------------
                // Module : Add Velocity
                // -------------------------
                if (MODULE.arrModuleCheck[3])
                {
                    // 0: From Center  
                    if (MODULE.AddVelocityType == 0)
                    {
                        float3 vDir = normalize(PARTICLE.vLocalPos.xyz);
                        PARTICLE.vVelocity.xyz = vDir * lerp(MODULE.SpeedMin, MODULE.SpeedMax, vRand.z);
                    }
                    // 1: To Center
                    else if (MODULE.AddVelocityType == 1)
                    {
                        float3 vDir = -normalize(PARTICLE.vLocalPos.xyz);
                        PARTICLE.vVelocity.xyz = vDir * lerp(MODULE.SpeedMin, MODULE.SpeedMax, vRand.z);
                        
                        // 중간으로 모이고 난 이후 삭제 필요함
                    }
                    // 2: Fix Direction
                    else
                    {
                        PARTICLE.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                    }
                    
                    // break;
                }
                
                // -------------------------
                // Module : Particle
                // -------------------------
                if (MODULE.arrModuleCheck[7])
                {
                    
                }
            }
        }
    }
    // case: Active
    else
    {
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        vUV.x += g_time * 0.2f;
        vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;

        float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0.f);
        
        // init force (frame별로 힘 적용)
        PARTICLE.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        // update Age
        PARTICLE.Age += g_dt;
        PARTICLE.NormalizedAge = PARTICLE.Age / PARTICLE.Life;
        
        if (PARTICLE.Life < PARTICLE.Age)
        {
            PARTICLE.Active = 0;
            return;
        }
        
        // -------------------------
        // Module : Scale
        // -------------------------
        if (MODULE.arrModuleCheck[2])
        {
            PARTICLE.vWorldScale = PARTICLE.vWorldInitScale * ((MODULE.vScaleRatio - 1.f) * PARTICLE.NormalizedAge + 1.f);
        }
        
        // -------------------------
        // Module : Nosie Force
        // -------------------------
        if (MODULE.arrModuleCheck[4])
        {
            if (PARTICLE.NosieForceTime == 0.f)
            {
                PARTICLE.vNosieForce = normalize(vRand.xyz * 2.f - 1.f) * MODULE.NosieForceScale;
                PARTICLE.NosieForceTime = g_time;
            }
            else if (MODULE.NosieForceTerm < g_time - PARTICLE.NosieForceTime)
            {
                PARTICLE.vNosieForce = normalize(vRand.xyz * 2.f - 1.f) * MODULE.NosieForceScale;
                PARTICLE.NosieForceTime = g_time;
            }
        }
        
        // -------------------------
        // Module : Calculate Force
        // -------------------------
        if (MODULE.arrModuleCheck[5])
        {
            // force
            PARTICLE.vForce.xyz += PARTICLE.vNosieForce.xyz;
            
            // accel
            float3 vAccel = PARTICLE.vForce.xyz / PARTICLE.Mass;
            if (MODULE.Gravity)
                vAccel += MODULE.GravityScale;
            
            // velocity
            PARTICLE.vVelocity.xyz += vAccel * g_dt;
            
            // -------------------------
            // Module : Drag
            // -------------------------
            if (MODULE.arrModuleCheck[1])
            {
                float limitTime = MODULE.DragTime - PARTICLE.Age;
                
                if (limitTime <= 0.f)
                    PARTICLE.vVelocity = 0.f;
                else
                {
                    float DT = g_dt / limitTime;
                    PARTICLE.vVelocity -= PARTICLE.vVelocity * DT;
                }
            }
            
            // position
            // Space Type - Local : 
            if (MODULE.SpaceType == 0)
            {
                PARTICLE.vLocalPos.xyz += PARTICLE.vVelocity.xyz * g_dt;
                PARTICLE.vWorldPos.xyz = PARTICLE.vLocalPos.xyz + CENTER_POS;
            }
            // Space Type - World : 
            else
                PARTICLE.vWorldPos.xyz += PARTICLE.vVelocity.xyz * g_dt;
        }
    }
}

#endif