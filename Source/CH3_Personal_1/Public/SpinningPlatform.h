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
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
};
