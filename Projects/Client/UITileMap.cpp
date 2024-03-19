#include "pch.h"
#include "UITileMap.h"

UITileMap::UITileMap()
	: UIComponent("TileMap##", "TileMap", COMPONENT_TYPE::TILEMAP)
{
}

UITileMap::~UITileMap()
{
}

void UITileMap::render_update()
{
	if (not GetTargetObject())
		return;
}