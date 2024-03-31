#pragma once
#include <Engine/CTexture.h>
#include "CImGuiMgr.h"
// =======================================
// UIParam: Mat로 전달할 Paramter UI
// =======================================
// - TEX_PARAM, SCALAR_PARAM
// - PARAM 전달용으로 Shader에 깡통 Mat를 만들어 전달하기도 했음

class UI;

class UIParam
{
private:
	static int	g_ID;

public:
	static void ResetID() { g_ID = 0; }
	static bool Param_INT(int* _Data, const string& _Desc);
	static bool Param_FLOAT(float* _Data, const string& _Desc);
	static bool Param_VEC2(Vec2* _Data, const string& _Desc);
	static bool Param_VEC3(Vec3* _Data, const string& _Desc);
	static bool Param_VEC4(Vec4* _Data, const string& _Desc);
	static bool Param_TEXTURE(_Inout_ Ptr<CTexture>& _Texture, const string& _Desc, UI* _Inst = nullptr, DELEGATE_1 _Func = nullptr);
};

