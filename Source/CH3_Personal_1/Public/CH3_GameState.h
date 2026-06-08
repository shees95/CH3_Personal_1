#pragma once

#include "CoreMinimal.h"
#include "SpawnLevelTable.h"
#include "GameFramework/GameStateBase.h"
#include "CH3_GameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	ReadyLevel,
	
	WaitingNextWave,
	
	InProgress,
	
	EndWave,
	EndLevel
};

UCLASS()
class CH3_PERSONAL_1_API ACH3_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACH3_GameState();
	
	virtual void BeginPlay() override;
	
	// 레벨 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	TArray<FName> LevelMapNames;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	float LevelDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 MaxLevels;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 CurrentLevelIndex;
	
	// 웨이브 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UDataTable> LevelDataTable;
	
	TArray<FSpawnLevelTable*> AllLevelInfo;
	TArray<FSpawnWaveTable> WavesInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	float WaitWaveDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 MaxWaves;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 CurrentWaveIndex;
	
	FTimerHandle WaveTimerHandle;
	
	// 점수 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 CollectedCoinCount;
	
	// 레벨 관련
	void OnGameOver();
	
	// 웨이브 관련
	void OnStartWave();
	void OnWaveTimeUp();
	
	// 점수 관련
	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const { return Score; };
	
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
	
	void OnCoinCollected();
	
	// State 처리
	void SetState(EWaveState newState);
	
	void ReadyLevel();
	void WaitingNextWave();
	void InProgress();
	void EndWave();
	void EndLevel();
	
	
	
	
	
	
	
	
};
