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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;
	
	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const { return Score; };
	
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
};
