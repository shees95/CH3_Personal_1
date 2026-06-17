#include "CH3_GameState.h"

#include "CH3_CharacterBase.h"
#include "CH3_GameInstance.h"
#include "CoinItem.h"
#include "SpawnVolumn.h"
#include "CH3_ControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnLevelTable.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"

ACH3_GameState::ACH3_GameState()
{
	WaveDuration = 30.f;
	WaitWaveDuration = 5.f;
	
	Score = 0;
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
}

void ACH3_GameState::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->GetFName() == FName("Main")) return;
	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ACH3_GameState::UpdateHUD, 0.1, true);
	SetState(EWaveState::ReadyLevel);
}

void ACH3_GameState::AddScore(int32 Amount)
{
	Score += Amount;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance);
		if (CH3_GameInstance)
		{
			CH3_GameInstance->AddToScore(Amount);
			UpdateHUD();
		}
	}
	
}

void ACH3_GameState::OnCoinCollected(ACoinItem* Item)
{
	CollectedCoinCount++;
	AddScore(Item->GetItemValue());
	UpdateHUD();
	UE_LOG(LogTemp, Warning, TEXT("Collected Coin : %d / %d"), CollectedCoinCount, SpawnedCoinCount);
	
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void ACH3_GameState::SetState(EWaveState newState)
{
	if (CurrentState == EWaveState::GameOver) return;
	
	CurrentState = newState;
	
	switch (newState)
	{
	case EWaveState::ReadyLevel:
		ReadyLevel();
		break;
		
		
	case EWaveState::WaitingNextWave:
		WaitingNextWave();
		break;
		
		
	case EWaveState::InProgress:
		InProgress();
		break;
		
		
	case EWaveState::EndWave:
		EndWave();
		break;
	case EWaveState::EndLevel:
		EndLevel();
		break;
		
	case EWaveState::GameOver:
		GameOver();
		break;
		
	}
}

void ACH3_GameState::OnStartWave()
{
	SetState(EWaveState::InProgress);
}

void ACH3_GameState::OnWaveTimeUp()
{
	SetState(EWaveState::EndWave);
}

void ACH3_GameState::GameOver()
{
	// 더이상 추가 행동 없음
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Game Over"));
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	
	UpdateHUD();
}

void ACH3_GameState::ReadyLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("ReadyLevel"));
	
	// 현재 레벨 체크
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance);
		if (CH3_GameInstance)
		{
			CurrentLevelIndex = CH3_GameInstance->CurrentLevelIndex;
			UE_LOG(LogTemp, Warning, TEXT("ReadyLevel::SetCurrentLevelIndex"));
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ReadyLevel::GetDataTable"));
	// 웨이브 정보 추출
	if (!LevelDataTable) return;
	
	// 웨이브 정보 추출
	LevelDataTable->GetAllRows("Error on StartLevel()", AllLevelInfo);
	
	if (CurrentLevelIndex >= AllLevelInfo.Num())
	{
		SetState(EWaveState::GameOver);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ReadyLevel::GetLevelInfo"));
	FSpawnLevelTable* LevelInfo = AllLevelInfo[CurrentLevelIndex];

	MaxLevels = AllLevelInfo.Num();
	
	UE_LOG(LogTemp, Warning, TEXT("ReadyLevel::GetWaveInfo"));
	WavesInfo = LevelInfo->Waves;
	MaxWaves = LevelInfo->Waves.Num();
	
	CurrentWaveIndex = 0;
	
	UpdateHUD();
	SetState(EWaveState::WaitingNextWave);
}

void ACH3_GameState::WaitingNextWave()
{
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &ACH3_GameState::OnStartWave, WaitWaveDuration, false);
}

void ACH3_GameState::InProgress()
{
	UE_LOG(LogTemp, Warning, TEXT("InProgress"));
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	
	// 레벨 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	
	
	TArray<AActor*> FoundVolumns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolumn::StaticClass(), FoundVolumns);
	
	if (WavesInfo.IsEmpty()) return;
	
	const int32 ItemToSpawn = WavesInfo[CurrentWaveIndex].MaxItems;
	
	WaveDuration = WavesInfo[CurrentWaveIndex].MaxLevelDuration;
	
	// 실제 스폰
	ASpawnVolumn* SpawnVolumn = Cast<ASpawnVolumn>(FoundVolumns[0]);
	
	if (FoundVolumns.Num() <= 0 || !SpawnVolumn) return;
	SpawnVolumn->SetSpawnWaveInfo(WavesInfo[CurrentWaveIndex]);
	
	for (int32 i = 0; i<ItemToSpawn;i++)
	{
		AActor* SpawnedActor = SpawnVolumn->SpawnRandomItem();
		
		// 코인 개수만 카운트
		if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
		{
			SpawnedCoinCount++;
		}
		
	}
	
	
	UE_LOG(LogTemp, Warning, TEXT("Spawned Coin : %d"), SpawnedCoinCount);
	// 타임 오버
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &ACH3_GameState::OnWaveTimeUp, WaveDuration, false);
}

void ACH3_GameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	
	// 코인 미달 - 패배 처리
	if (CollectedCoinCount < SpawnedCoinCount)
	{
		SetState(EWaveState::GameOver);
		return;
	}
	
	SetState(EWaveState::EndLevel);
	
}

void ACH3_GameState::EndLevel()
{
	// 생존 시 다음 웨이브
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance))
		{
			if (CurrentWaveIndex + 1 >= MaxWaves)
			{
				
				if (CurrentLevelIndex + 1 >= MaxLevels)
				{
					// 모든 레벨 종료
					SetState(EWaveState::GameOver);
				}
				else
				{
					// 다음 레벨
					CH3_GameInstance->CurrentLevelIndex = ++CurrentLevelIndex;
					UGameplayStatics::OpenLevel(GetWorld(), AllLevelInfo[CurrentLevelIndex]->LvlName);
					
				}
			}
			else
			{
				// 다음 웨이브
				CurrentWaveIndex++;
				SetState(EWaveState::WaitingNextWave);
			}
			
		}
	}
}

void ACH3_GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3_ControllerBase* CH3_Controller =Cast<ACH3_ControllerBase>(PlayerController))
		{
			if (UUserWidget* HUDWidget = CH3_Controller->GetHUDWidget())
			{
				if (UProgressBar* TimeBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("TimeBar"))))
				{
					float TimerRate = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle) / GetWorldTimerManager().GetTimerRate(WaveTimerHandle);
					TimeBar->SetPercent(TimerRate);
				}
				
				if (UTextBlock* LevelTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelTextBlock"))))
				{
					LevelTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Level : %d, Wave : %d"), CurrentLevelIndex + 1, CurrentWaveIndex + 1)));
					
				}
				
				if (UTextBlock* ScoreTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreTextBlock"))))
				{
					ScoreTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), Score)));
				}
				
				
				if (URadialSlider* HPBar = Cast<URadialSlider>(HUDWidget->GetWidgetFromName(TEXT("HPBar"))))
				{
					//  60 ~ 120
					APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
					ACH3_CharacterBase* PlayerCharacter = Cast<ACH3_CharacterBase>(Player);
					float RemainingHPPercent = PlayerCharacter->GetHealth() / PlayerCharacter->GetMaxHealth();
					
					HPBar->SetSliderHandleStartAngle(60);
					HPBar->SetSliderHandleEndAngle(60 * RemainingHPPercent + 60);
				}
				
				if (URadialSlider* ScoreBar = Cast<URadialSlider>(HUDWidget->GetWidgetFromName(TEXT("ScoreBar"))))
				{
					//  240 ~ 300
					if (SpawnedCoinCount > 0)
					{
						float RemainingCoins = (float)CollectedCoinCount / (float)SpawnedCoinCount;
						
						ScoreBar->SetSliderHandleStartAngle(300 - 60 * RemainingCoins);
						ScoreBar->SetSliderHandleEndAngle(300);
					}
				}
			}
		}
	}
}
