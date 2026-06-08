#include "CH3_GameInstance.h"

UCH3_GameInstance::UCH3_GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
	CurrentWaveIndex = 0;
}

void UCH3_GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Update : %d"), TotalScore);
}


