#include "CH3_GameState.h"

ACH3_GameState::ACH3_GameState()
{
	Score = 0;
}

void ACH3_GameState::AddScore(int32 Amount)
{
	Score += Amount;
}
