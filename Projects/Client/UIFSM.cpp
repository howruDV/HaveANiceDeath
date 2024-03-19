#include "pch.h"
#include "UIFSM.h"

UIFSM::UIFSM()
	: UIAsset("FSM", "##FSM", ASSET_TYPE::FSM)
{
}

UIFSM::~UIFSM()
{
}

void UIFSM::render_update()
{
	if (not GetAsset().Get())
		return;

	UIAsset::render_update();

}