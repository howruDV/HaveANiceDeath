#pragma once
#include <Engine\singleton.h>

// =======================================
// CEditorObjMgr: Editor Àü¿ë Mgr
// =======================================
class CEditorObj;

class CEditorObjMgr :
    public CSingleton<CEditorObjMgr>
{
    SINGLE(CEditorObjMgr)

private:
    vector<CEditorObj*>    m_vecEditorObj;

public:
    void init();
    void progress();
};
