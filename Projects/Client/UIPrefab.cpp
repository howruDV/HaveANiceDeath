#include "pch.h"
#include "UIPrefab.h"

UIPrefab::UIPrefab()
	: UIAsset("Prefab", "##Prefab", ASSET_TYPE::PREFAB)
{
}

UIPrefab::~UIPrefab()
{
}

void UIPrefab::render_update()
{
    if (not GetAsset().Get())
        return;
}