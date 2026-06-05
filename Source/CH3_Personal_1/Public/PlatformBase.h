// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"

UCLASS()
class CH3_PERSONAL_1_API APlatformBase : public AActor
{
	GENERATED_BODY()
protected:
	
	FTransform OriginTransform; 
	
	UPROPERTY(EditAnywhere)
	bool IsRandomSpawn = false;
	
	
	
public:	
	APlatformBase();

protected:
	virtual void BeginPlay() override;

public:	

};
