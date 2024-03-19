#pragma once
#include "CAsset.h"
// =======================================
// CTexture: 이미지 리소스 텍스쳐로 생성 및 관리
// =======================================

class CTexture :
    public CAsset
{
private:
    ScratchImage                        m_Image;    // System memory
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // GPU memory
    D3D11_TEXTURE2D_DESC                m_Desc;     // 텍스쳐 생성정보

    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;      // texture register(t) - for Shader
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;      // GPGPU(General Purpose GPU) - for ComputeShader

    UINT                                m_RecentCSRegNum_SRV;
    UINT                                m_RecentCSRegNum_UAV;

private:
    virtual int Load(const wstring& _strFilePath) override;
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _USAGE);
    int Create(ComPtr<ID3D11Texture2D> _tex2D);

public:
    void UpdatePipeline(int _registerNum);
    int UpdateCS_SRV(int _registerNum);
    int UpdateCS_UAV(int _registerNum);
    static void ClearPipeline(int _registerNum);
    void ClearCS_SRV();
    void ClearCS_UAV();

    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }
    ComPtr<ID3D11Texture2D> GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_UAV; }
    FPixel* GetPixels();

public:
    CLONE_DISABLE(CTexture);
    CTexture(bool _Engine = false);
    ~CTexture();
    friend class CAssetMgr;
};