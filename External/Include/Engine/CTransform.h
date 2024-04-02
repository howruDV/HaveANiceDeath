#pragma once
#include "CComponent.h"

// =======================================
// CTransform: Object �̵� ��� ����, ����
// =======================================
// - �̵�: constant buffer�� transform �� �Űž� ��

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos_Prev;
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRotation;    // Hierachy structure, �θ��� ��ġ ����� ��
    Vec3    m_arrLocalDir[3];            // Right, Up, Front (��ȣ�ٲٸ� �ǹǷ� �� ���� �˸� 6������ �� �� �� ����)
    Vec3    m_arrWorldDir[3];       // local dir: obj ����, world dir: obj���� world�� ����
    Matrix  m_matWorld;
    bool    m_bAbsolute;

public:
    virtual void finaltick() override;
    virtual void UpdatePipeline() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:    
    void SetRelativePos(Vec3 _Pos) { m_vRelativePos = _Pos; }
    void SetRelativePos_Prev(Vec3 _Pos) { m_vRelativePos_Prev = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_vRelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rotation) { m_vRelativeRotation = _Rotation; }
    void SetAbsolute(bool _bAbsolute) { m_bAbsolute = _bAbsolute; }
    void SetWorldMat(const Matrix& _mat) { m_matWorld = _mat; }

    Vec3 GetRelativePos_Prev() { return m_vRelativePos_Prev; }
    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRelativeRotation; }
    Vec3 GetLocalDir(DIR_TYPE _Type) { return m_arrLocalDir[(UINT)_Type]; }
    const Matrix& GetWorldMat() { return m_matWorld; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();
    //Vec3 GetWorldRotate(); // Rotation�� world ��� �����... ���߿�!
    Vec3 GetWorldDir(DIR_TYPE _Type) { return m_arrWorldDir[(UINT)_Type]; }
    bool IsAbsolute() { return m_bAbsolute; }

public:
    CLONE(CTransform);
    CTransform();
    ~CTransform();
};