#pragma once
#include "CTexture.h"

// =======================================
// CDevice: DRX11 Graphic Device
// =======================================
class CConstBuffer;

class CDevice :
    public CSingleton<CDevice>
{
    SINGLE(CDevice);

private:
    ComPtr<ID3D11Device>            m_Device;       // GPU 메모리 관리 & 객체 생성
    ComPtr<ID3D11DeviceContext>     m_Context;      // GPU 렌더링 명령
    ComPtr<IDXGISwapChain>          m_SwapChain;    // 출력 버퍼 지정

    // OM (Ouput Merge State)
    Ptr<CTexture>                   m_RTTex;
    Ptr<CTexture>                   m_DSTex;
    //ComPtr<ID3D11Texture2D>         m_RTTex;        // Render Target
    //ComPtr<ID3D11RenderTargetView>  m_RTView;
    //ComPtr<ID3D11Texture2D>         m_DSTex;        // Depth Stencil
    //ComPtr<ID3D11DepthStencilView>  m_DSView;
    HWND                            m_hRenderWnd;
    Vec2                            m_vRenderResolution;

    ComPtr<ID3D11RasterizerState>   m_arrRS[(UINT)RS_TYPE::END];
    ComPtr<ID3D11DepthStencilState> m_arrDS[(UINT)DS_TYPE::END];
    ComPtr<ID3D11BlendState>        m_arrBS[(UINT)BS_TYPE::END];
    ComPtr<ID3D11SamplerState>      m_arrSampler[(UINT)SAMPLER_TYPE::END];
    
    // Constant Buffer
    CConstBuffer*                   m_arrCB[(UINT)CB_TYPE::END];

private:
    // pipeline
    int CreateSwapChain();
    int CreateTargetView();
    int CreateRasterizerState();
    int CreateDepthStencilState();
    int CreateBlendState();
    int CreateSamplerState();

    // const buffer
    int CreateConstBuffer();

    // bloom
    int CreateBloomTex();

public:
    int init(HWND _hWnd, Vec2 _vResolution);
    void SetRenderTarget();
    void ClearRenderTarget();
    void ClearRenderTarget(float(&Color)[4]);
    void Present();
    
public:
    ID3D11Device* GetDevice() { return m_Device.Get(); }
    ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
    Vec2 GetRenderResolution() { return m_vRenderResolution; }
    ComPtr<ID3D11RasterizerState> GetRState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
    ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
    ComPtr<ID3D11BlendState> GetBState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }
    CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
};