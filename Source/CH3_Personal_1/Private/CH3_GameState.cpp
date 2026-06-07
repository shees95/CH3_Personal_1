#include "CH3_GameState.h"

#include "CH3_GameInstance.h"
#include "CoinItem.h"
#include "SpawnVolumn.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolumn.h"
ACH3_GameState::ACH3_GameState()
{
	Score = 0;
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	LevelDuration = 30.f;
	MaxLevels = 3;
	CurrentLevelIndex = 0;
}

void ACH3_GameState::BeginPlay()
{
	Super::BeginPlay();
	
	StartLevel();
}

void ACH3_GameState::AddScore(int32 Amount)
{
	Score += Amount;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance);
		if (CH3_GameInstance)
		{
			CH3_GameInstance->AddToScore(Score);
		}
	}
	
}

void ACH3_GameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance);
		if (CH3_GameInstance)
		{
			CurrentLevelIndex = CH3_GameInstance->CurrentLevelIndex;
		}
	}
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	TArray<AActor*> FoundVolumns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolumn::StaticClass(), FoundVolumns);
	
	const int32 ItemToSpawn = 40;
	
	for (int32 i = 0; i<ItemToSpawn;i ++)
	{
		if (FoundVolumns.Num() > 0)
		{
			ASpawnVolumn* SpawnVolumn = Cast<ASpawnVolumn>(FoundVolumns[0]);
			if (SpawnVolumn)
			{
				AActor* SpawnedActor = SpawnVolumn->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Spawned Coin : %d"), SpawnedCoinCount);
	
	GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &ACH3_GameState::OnLevelTimeUp, LevelDuration, false);
	
	
}

void ACH3_GameState::OnLevelTimeUp()
{
	EndLevel();
}

void ACH3_GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance);
		if (CH3_GameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			CH3_GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (CollectedCoinCount < SpawnedCoinCount)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ACH3_GameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}

void ACH3_GameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Collected Coin : %d / %d"), CollectedCoinCount, SpawnedCoinCount);
	
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}
