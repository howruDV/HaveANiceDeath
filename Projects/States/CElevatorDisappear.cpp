#include "pch.h"
#include "CElevatorDisappear.h"

CElevatorDisappear::CElevatorDisappear()
	: CState(ELEVATORDISAPPEAR)
{
}

CElevatorDisappear::~CElevatorDisappear()
{
}

void CElevatorDisappear::finaltick()
{
}

void CElevatorDisappear::Enter()
{
	GetOwner()->Animator2D()->Play(L"Open", false, true);
	GamePlayStatic::Play2DSound(L"sound\\obj\\elev\\GPE_Elevator_Close_Down_01.wav", 1, 0.3f);
}

void CElevatorDisappear::Exit()
{
}
