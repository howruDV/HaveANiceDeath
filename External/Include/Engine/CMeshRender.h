#pragma once
#include "CRenderComponent.h"

// =======================================
// CMeshRender: Mesh render 기능 담당
// =======================================

class CMeshRender :
    public CRenderComponent
{
public: 
    virtual void finaltick() override;
    virtual void render() override;
    virtual void UpdatePipeline() override;

public:
    CLONE(CMeshRender);
    CMeshRender();
    ~CMeshRender();
};

