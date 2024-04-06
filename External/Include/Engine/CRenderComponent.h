#pragma once
#include "CComponent.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "Ptr.h"

// =======================================
// CRenderComponent: renderComponents�� ����ϴ� parent class
// =======================================

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_Mesh;
    // dynmaic material: �б��Ȳ�� �߻��ϴ� ���, ������ ������ ���� ���� ���� ����
    Ptr<CMaterial>          m_SharedMat;
    Ptr<CMaterial>          m_DynamicMat;
    Ptr<CMaterial>          m_CurMat;

public:
    virtual void begin() override;
    virtual void finaltick() {};
    virtual void UpdatePipeline() = 0;
    virtual void render() = 0;

    virtual CRenderComponent* Clone() = 0;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    
    void RestoreMaterial();

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mat);
    Ptr<CMesh> GetMesh() { return m_Mesh; }
    Ptr<CMaterial> GetMaterial() { return m_CurMat; }
    Ptr<CMaterial> GetSharedMaterial() { return m_SharedMat; }
    Ptr<CMaterial> GetDynamicMaterial();

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _OriginRenderCom);
    ~CRenderComponent();
};

