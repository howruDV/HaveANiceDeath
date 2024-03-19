#pragma once
#include "CComponent.h"
// =======================================
// CCamera: ViewSpace �̵� �� Projection ���
// =======================================
// - Space: ī�޶� ��ġ�� ��������, ī�޶� ���� ������ Z������ ��
// - Matrix: world space -(view matrix)-> view space -(projection matrix)-> screen space
// - Camera�� component�̹Ƿ� ������ ���� �� ����

enum class PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE
};

class CCamera :
    public CComponent
{
private:
    PROJ_TYPE   m_ProjType;
    int         m_CameraPriority;

    // Perspective projection
    float m_FOV;            // �þ߰� Field of View

    // Orthographic projection
    float m_Width;
    float m_Scale;          // ����

    // Both
    float m_AspectRatio;    // ���� ��Ⱦ��
    float m_Far;            // ī�޶� �ִ�Ÿ�

    // Matrix
    Matrix m_matView;
    Matrix m_matProj;

    // Layer Check
    UINT m_LayerCheck;      // camera�� �� �� �ִ� layer ���� (bit flag)

    // Object Sort (rendering order)
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMasked;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;

private:
    void render(vector<CGameObject*>& _vec);
    void render_postprocess();

public:
    virtual void begin() override;
    virtual void finaltick() override;
    void render();

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void SortObject();

public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetScale(float _Scale) { m_Scale = _Scale; }
    void SetFar(float _Far) { m_Far = _Far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }
    void SetWidth(float Width) { m_Width = Width; }
    void SetAspectRadio(float _radio) { m_AspectRatio = _radio; }
    void SetCameraPriority(int _Priority);
    void LayerCheck(UINT _LayerIdx, bool _bCheck);
    void LayerCheck(const wstring& _strLayerName, bool _bCheck);
    void LayerCheckAll(){ m_LayerCheck = 0xffffffff; }

    PROJ_TYPE GetProjType() { return m_ProjType; }
    float GetScale() { return m_Scale; }
    float GetFar() { return m_Far; }
    float GetFOV() { return m_FOV; }
    float GetWidth() { return m_Width; }
    float GetAspectRadio() { return m_AspectRatio; }
    Matrix& GetViewMat() { return m_matView; }
    Matrix& GetProjMat() { return m_matProj; }

public:
    CLONE(CCamera);
    CCamera();
    ~CCamera();
};

