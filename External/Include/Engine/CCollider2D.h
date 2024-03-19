#pragma once
#include "CComponent.h"
// =======================================
// CCollider2D: 충돌체를 생성, 관리하는 Components
// =======================================
// - 물체는 충돌할 수 있게 됨(충돌체를 가짐)

enum class COLLIDER2D_TYPE
{
    RECT,
    CIRCLE,
};

class CCollider2D :
    public CComponent
{
    COLLIDER2D_TYPE m_Type;
    Vec3            m_vOffsetPos;       // 오브젝트로부터 상대적인 위치
    Vec3            m_vOffsetScale;     // 오브젝트로부터 상대적인 배율
    Vec3            m_vOffestRotation;  // 오브젝트로부터 상대적인 회전(자전)
    Vec3            m_FinalPos;         // 충돌체의 최종 위치 ( Object Position + Collider offset )
    Matrix          m_matColWorld;

    bool            m_bAbsolute;        // true인 경우, Owner의 Scale 영향 받지 않음
    int             m_CollisionCount;   // 충돌중인 갯수

    bool            m_Active;           // 충돌체의 활성화 여부
    bool            m_SemiDeactive;     // 비활성화 예정상태

public:
    virtual void finaltick() override;
    void BeginOverlap(CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Activate() { m_Active = true; }
    void Deactivate();

public:
    void SetColliderType(COLLIDER2D_TYPE _Type) { m_Type = _Type; }
    void SetOffsetPos(Vec3 _vOffset) { m_vOffsetPos = _vOffset; }
    void SetOffsetScale(Vec3 _vOffsetScale) { m_vOffsetScale = _vOffsetScale; }
    void SetRotationZ(float _Angle) { m_vOffestRotation.z = _Angle; }
    void SetAbsolute(bool _bAbsol) { m_bAbsolute = _bAbsol; }

    const Matrix& GetWorldMat() { return m_matColWorld; }
    COLLIDER2D_TYPE GetColliderType() { return m_Type; }
    Vec3 GetOffsetPos() { return m_vOffsetPos; }
    Vec3 GetOffsetScale() { return m_vOffsetScale; }
    Vec3 GetFinalPos() { return m_FinalPos; }
    float GetRotationZ() { return m_vOffestRotation.z; }
    int GetCollisionCount() { return m_CollisionCount; };
    bool IsActive() { return m_Active; }
    bool IsAbsolute() { return m_bAbsolute; }
    bool IsSemiDeactive() { return m_SemiDeactive; }

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _OriginCollider2D);
    ~CCollider2D();

    friend class CTaskMgr;
};

