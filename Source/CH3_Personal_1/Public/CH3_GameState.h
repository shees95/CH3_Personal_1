#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CH3_GameState.generated.h"


UCLASS()
class CH3_PERSONAL_1_API ACH3_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACH3_GameState();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 CollectedCoinCount;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	float LevelDuration;
	
	FTimerHandle LevelTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	TArray<FName> LevelMapNames;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 CurrentLevelIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 MaxLevels;

	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const { return Score; };
	
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
	
	void StartLevel();

	
	void OnLevelTimeUp();
	void EndLevel();
	void OnGameOver();
	
	void OnCoinCollected();
};
