#pragma once
#include "CComponent.h"
// =======================================
// CCollider2D: �浹ü�� ����, �����ϴ� Components
// =======================================
// - ��ü�� �浹�� �� �ְ� ��(�浹ü�� ����)

enum class COLLIDER2D_TYPE
{
    RECT,
    CIRCLE,
};

class CCollider2D :
    public CComponent
{
    COLLIDER2D_TYPE m_Type;
    Vec3            m_vOffsetPos;       // ������Ʈ�κ��� ������� ��ġ
    Vec3            m_vOffsetScale;     // ������Ʈ�κ��� ������� ����
    Vec3            m_vOffestRotation;  // ������Ʈ�κ��� ������� ȸ��(����)
    Vec3            m_FinalPos;         // �浹ü�� ���� ��ġ ( Object Position + Collider offset )
    Matrix          m_matColWorld;

    bool            m_bAbsolute;        // true�� ���, Owner�� Scale ���� ���� ����
    int             m_CollisionCount;   // �浹���� ����

    bool            m_Active;           // �浹ü�� Ȱ��ȭ ����
    bool            m_SemiDeactive;     // ��Ȱ��ȭ ��������

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

