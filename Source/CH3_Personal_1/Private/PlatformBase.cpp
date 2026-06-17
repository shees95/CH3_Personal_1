// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformBase.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
	if (IsRandomSpawn) AddActorWorldOffset(FVector(FMath::RandRange(-300,300), FMath::RandRange(-300,300), 0));
	OriginTransform = GetActorTransform();
	
	
	
}
