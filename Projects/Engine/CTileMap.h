#pragma once
#include "CRenderComponent.h"
// =======================================
// CTilemap: tile ������� Object�� �׸� �� �ְ� �ϴ� Component
// =======================================
// - tile�� ���Ƶ� �ѹ��� �׸� �� ����

class CStructuredBuffer;

struct FTileInfo
{
    // strucutred buffer: 16byte align
    Vec2 vLeftTopUV;
    int bRender;
    int padding;
};

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       m_Atlas;
    Vec2                m_vTileRenderSize;  // Tile�� render size
    Vec2                m_vTilePixelSize;   // Tile�� origin size
    Vec2                m_vSliceSizeUV;         // Tile�� UV
    UINT                m_MapCol;           // TileMap�� col
    UINT                m_MapRow;           // TileMap�� row
    UINT                m_AtlasCol;         // Atlas�� col
    UINT                m_AtlasRow;         // Atlas�� row

    vector<FTileInfo>   m_vecTileInfo;
    CStructuredBuffer*  m_TileInfoBuffer;

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void UpdatePipeline() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetAtlas(Ptr<CTexture> _Atlas, Vec2 _TilePixelSize);
    void SetMap(UINT _Row, UINT _Col);
    void SetTileImgIdx(UINT _Row, UINT _Col, UINT _ImgIdx);
    Ptr<CTexture> GetAtlas() { return m_Atlas; }
    UINT GetMapCol() { return m_MapCol; }
    UINT GetMapRow() { return m_MapRow; }

public:
    CLONE(CTileMap);
    CTileMap();
    CTileMap(const CTileMap& _OriginTileMap);
    ~CTileMap();
};

