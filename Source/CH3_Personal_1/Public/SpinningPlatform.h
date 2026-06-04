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
	
	float TimeToAppear = 1.f;
	float TimeToDisappear = 3.5f;
	FTimerHandle BlickTimerHandle;
	
public:	
	virtual void Tick(float DeltaTime) override;
	void PlatformDisappear();
	void PlatformAppear();
	
};
