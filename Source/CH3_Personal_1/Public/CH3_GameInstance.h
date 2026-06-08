#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CH3_GameInstance.generated.h"


UCLASS()
class CH3_PERSONAL_1_API UCH3_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCH3_GameInstance();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GameData")
	int32 TotalScore;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GameData")
	int32 CurrentLevelIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="GameData")
	int32 CurrentWaveIndex;
	
	UFUNCTION(BlueprintCallable, Category="GameData")
	void AddToScore(int32 Amount);
	
};
