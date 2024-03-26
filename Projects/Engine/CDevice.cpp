#include "pch.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAssetMgr.h"

CDevice::CDevice()
    : m_hRenderWnd(nullptr)
    , m_arrCB{}
{

}

CDevice::~CDevice()
{
    Delete_Array(m_arrCB);
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hRenderWnd = _hWnd;
    m_vRenderResolution = _vResolution;

    // --------------------------
    // (1) Device & DeviceContext 생성
	// --------------------------
    // device initialize
    D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;
    if (FAILED( D3D11CreateDevice(nullptr
                                , D3D_DRIVER_TYPE_HARDWARE, nullptr
                                , D3D11_CREATE_DEVICE_DEBUG
                                , nullptr, 0, D3D11_SDK_VERSION
                                , m_Device.GetAddressOf(), &eLevel, m_Context.GetAddressOf()) ))
    {
        MessageBox(nullptr, L"Fail to Create Device", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    // --------------------------
    // (2) SwapChain 생성
    // --------------------------
    // create swapChain
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"Fail to Create SwapChain", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    // --------------------------
    // (3) TargetView 설정 (DepthStencil & View)
    // --------------------------
    // (4) OM 설정
    // --------------------------
    // create render target, render target view, depth stencil, depth stencil view
    if (FAILED(CreateTargetView()))
    {
        MessageBox(nullptr, L"Fail to Create Target&View", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    // default로 선택되는 state들 추가구현: Rasterizer, DepthStencil, Blend State
    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"Fail to Create Rasterizer State", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"Fail to Create DepthStencil State", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"Fail to Create Blend State", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSamplerState()))
    {
        MessageBox(nullptr, L"Fail to Create Sampler State", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    // --------------------------
    // (5) Viewport setting
    // --------------------------
    D3D11_VIEWPORT ViewportDesc = {};

    ViewportDesc.MaxDepth = 0;
    ViewportDesc.MaxDepth = 1.f;
    ViewportDesc.TopLeftX = 0;
    ViewportDesc.TopLeftY = 0;
    ViewportDesc.Width = m_vRenderResolution.x;
    ViewportDesc.Height = m_vRenderResolution.y;

    CONTEXT->RSSetViewports(1, &ViewportDesc);

    // constant buffer 생성
    if (FAILED(CreateConstBuffer()))
    {
        MessageBox(nullptr, L"Fail to Create ConstantBuffer", L"Fail to Initialize Device", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void CDevice::ClearRenderTarget(float(&Color)[4])
{
    // RenderTargetTexture 색상값 초기화
    // - Color: Normalized Value (0~255 X , 0~1 O)
    // - Depth: 0~1, 1로 갈수록 멀어짐
    m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), Color);
    m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
    // Present() Buffer 그림
    m_SwapChain->Present(0, 0);
}

int CDevice::CreateSwapChain()
{
    // SwapChain 생성 구조체
    // - SwapChain이 관리하는 Render Target Buffer의 구성 정보
    DXGI_SWAP_CHAIN_DESC tDesc = {};
    tDesc.BufferCount = 1; // 개념적 버퍼는 하나
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32bit
    tDesc.BufferDesc.RefreshRate.Denominator = 1;
    tDesc.BufferDesc.RefreshRate.Numerator = 60;
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    tDesc.Flags = 0;
    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Windowed = true; // 창모드
    tDesc.OutputWindow = m_hRenderWnd;

    // Access Factor 
    // - Factory: swapChain 생성 기능
    // - SwapChain 생성 과정: Device -> Adapter -> Factory -> Swapchain
    ComPtr<IDXGIDevice> pldxgiDevice = nullptr;
    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    ComPtr<IDXGIFactory> pFactory = nullptr;
    m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pldxgiDevice.GetAddressOf());
    pldxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    // Create SwapChain
    if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf())))
        return E_FAIL;

    // 전역 데이터에 렌더링 해상도 기록
    g_Global.g_RenderResolution = m_vRenderResolution;

    return S_OK;
}

int CDevice::CreateTargetView()
{
    // Get Render Target Texture & View
    ComPtr<ID3D11Texture2D> tex2D;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

    //m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());
    m_RTTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

    // Create Depth Stencil Texture
    m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
    
    /*
    D3D11_TEXTURE2D_DESC Desc = {};
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;// depth 24bit + stencil 8bit
    Desc.Width = (UINT)m_vRenderResolution.x;   // depth stencil state texture 해상도는 renter target texture 해상도와 같아야 함
    Desc.Height = (UINT)m_vRenderResolution.y;
    Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;  // 용도
    Desc.CPUAccessFlags = 0;                    // CPU 접근 불가
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.SampleDesc.Count = 1;                  // sampling
    Desc.SampleDesc.Quality = 0;
    Desc.MipLevels = 1;                         // 저퀄리티 버전 사본 생성여부
    Desc.MiscFlags = 0;
    Desc.ArraySize = 1;

    if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTex.GetAddressOf())))
        return E_FAIL;

    // Get Depth Stencil View
    m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());
    */

    // Output Merge State(OM)에 Render Target Texture와 Depth Stencil Texture 전달
    //m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get()); // ImGUI 붙이면서 renderTarget이 중간에 바뀔 수 있으므로 renderMgr로 옮겨줌

    return S_OK;
}

// Rasterizer: Geometry Shader 이후, Pixel Shader 이전
int CDevice::CreateRasterizerState()
{
    HRESULT result = E_FAIL;
    D3D11_RASTERIZER_DESC tDesc = {};

    // Cull back: default (알아서 디폴트모드 지정!)
    m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    // Cull front: CW
    tDesc.CullMode = D3D11_CULL_FRONT;
    tDesc.FillMode = D3D11_FILL_SOLID;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    // Cull none
    tDesc.CullMode = D3D11_CULL_NONE;
    tDesc.FillMode = D3D11_FILL_SOLID;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    // WireFrame
    tDesc.CullMode = D3D11_CULL_NONE;
    tDesc.FillMode = D3D11_FILL_WIREFRAME;
    result = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    return S_OK;
}

// Depth&Stencil State: OM
int CDevice::CreateDepthStencilState()
{
    HRESULT result = E_FAIL;
    D3D11_DEPTH_STENCIL_DESC tDesc;

    // Less Equal
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;
    result = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    // Greate Equal
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;
    result = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GREATER_EQAUL].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    // No Test
    tDesc.DepthEnable = false;
    tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    tDesc.StencilEnable = false;
    result = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    // No Write
    tDesc.DepthEnable = true;
    tDesc.DepthFunc = D3D11_COMPARISON_LESS; // default
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    tDesc.StencilEnable = false;
    result = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    // No Test No Write
    tDesc.DepthEnable = false;
    tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
    tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    tDesc.StencilEnable = false;
    result = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
    if (FAILED(result))
        return E_FAIL;

    return S_OK;
}

int CDevice::CreateBlendState()
{
    D3D11_BLEND_DESC tDesc = {};

    // default: 지정x (알아서 처리)
    m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

    // AlphaBlend
    tDesc.AlphaToCoverageEnable = false;
    tDesc.IndependentBlendEnable = true;

    tDesc.RenderTarget[0].BlendEnable = true;
    tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

    // ONE_ONE
    tDesc.AlphaToCoverageEnable = false;
    tDesc.IndependentBlendEnable = false;

    tDesc.RenderTarget[0].BlendEnable = true;
    tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

    return S_OK;
}

int CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC tDesc = {};

    // anisotropic 이방성 필터링
    tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    tDesc.MinLOD = 0;
    tDesc.MaxLOD = 1;

    DEVICE->CreateSamplerState(&tDesc, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());

    // mimap 밉맵 필터링
    tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    tDesc.MinLOD = 0;
    tDesc.MaxLOD = 1;

    DEVICE->CreateSamplerState(&tDesc, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());

    // pipeline sampler update
    CONTEXT->VSSetSamplers(0, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANIS].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());
    CONTEXT->CSSetSamplers(1, 1, m_arrSampler[(UINT)SAMPLER_TYPE::MIPMAP].GetAddressOf());

    return 0;
}

int CDevice::CreateConstBuffer()
{
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
    m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(FTransform), 1);

    m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST] = new CConstBuffer(CB_TYPE::MATERIAL_CONST);
    m_arrCB[(UINT)CB_TYPE::MATERIAL_CONST]->Create(sizeof(FMatConst), 1);

    m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA] = new CConstBuffer(CB_TYPE::ANIM2D_DATA);
    m_arrCB[(UINT)CB_TYPE::ANIM2D_DATA]->Create(sizeof(FAnimData2D), 1);

    m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA] = new CConstBuffer(CB_TYPE::GLOBAL_DATA);
    m_arrCB[(UINT)CB_TYPE::GLOBAL_DATA]->Create(sizeof(FGlobalData), 1);

    return S_OK;
}
