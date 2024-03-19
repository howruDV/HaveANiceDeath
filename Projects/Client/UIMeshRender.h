#pragma once
#include "UIComponent.h"
// =======================================
// UITransform: MeshRender component ¿¬°á
// =======================================

void MeshSelect(DWORD_PTR _ptr);
void MaterialSelect(DWORD_PTR _ptr);

class UIMeshRender :
    public UIComponent
{
public:
    virtual void render_update() override;
    void MeshSelect(DWORD_PTR _ptr);
    void MaterialSelect(DWORD_PTR _ptr);

public:
    UIMeshRender();
    ~UIMeshRender();
};

