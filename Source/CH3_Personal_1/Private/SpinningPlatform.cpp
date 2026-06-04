// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinningPlatform.h"

ASpinningPlatform::ASpinningPlatform()
{
}

void ASpinningPlatform::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASpinningPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void ASpinningPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorWorldRotation(FRotator(0,MoveSpeed * DeltaTime, 0));
	
}
