#pragma once
#include "CComponent.h"

// =======================================
// CTransform: Object 이동 기능 구현, 관리
// =======================================
// - 이동: constant buffer로 transform 값 옮거야 함

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos_Prev;
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRotation;    // Hierachy structure, 부모의 위치 따라야 함
    Vec3    m_arrLocalDir[3];            // Right, Up, Front (부호바꾸면 되므로 세 값만 알면 6방향의 값 알 수 있음)
    Vec3    m_arrWorldDir[3];       // local dir: obj 방향, world dir: obj속한 world내 방향
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
    //Vec3 GetWorldRotate(); // Rotation은 world 계산 어려움... 나중에!
    Vec3 GetWorldDir(DIR_TYPE _Type) { return m_arrWorldDir[(UINT)_Type]; }
    bool IsAbsolute() { return m_bAbsolute; }

public:
    CLONE(CTransform);
    CTransform();
    ~CTransform();
};