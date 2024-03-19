#pragma once

// Windows header
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
using std::wstring;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::make_pair;

#include <wrl.h>
using namespace Microsoft::WRL;

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;
using std::experimental::filesystem::path;

// C runtime header
#include <stdlib.h>

// DirectX 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
using namespace DirectX;

// DirectXTex
#include <DirectXTex/DirectXTex.h>
#ifdef  _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// Engine
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"