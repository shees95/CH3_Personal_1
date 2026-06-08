// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "SpinningPlatform.generated.h"

/**
 * 
 */
UCLASS()
class CH3_PERSONAL_1_API ASpinningPlatform : public APlatformBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 200.f;
	
public:	
	ASpinningPlatform();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	bool IsBlink = false;
	
	UPROPERTY(EditAnywhere)
	float TimeToAppear = 1.f;
	
	UPROPERTY(EditAnywhere)
	float TimeToDisappear = 3.5f;
	FTimerHandle BlickTimerHandle;
	
public:	
	virtual void Tick(float DeltaTime) override;
	void PlatformDisappear();
	void PlatformAppear();
	
};
