// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformBase.h"

// Sets default values
APlatformBase::APlatformBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
	if (IsRandomSpawn) AddActorWorldOffset(FVector(FMath::RandRange(-300,300), FMath::RandRange(-300,300), 0));
	OriginTransform = GetActorTransform();
	
	
	
}
