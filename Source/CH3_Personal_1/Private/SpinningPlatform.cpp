// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinningPlatform.h"

ASpinningPlatform::ASpinningPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 200.f;
}

void ASpinningPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(BlickTimerHandle, this, &ASpinningPlatform::PlatformAppear, TimeToDisappear);
	
}

void ASpinningPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalRotation(FRotator(0,MoveSpeed * DeltaTime, 0));
	
}

void ASpinningPlatform::PlatformDisappear()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(true);
	
	GetWorld()->GetTimerManager().SetTimer(BlickTimerHandle, this, &ASpinningPlatform::PlatformAppear, TimeToAppear);
}

void ASpinningPlatform::PlatformAppear()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	
	GetWorld()->GetTimerManager().SetTimer(BlickTimerHandle, this, &ASpinningPlatform::PlatformAppear, TimeToDisappear);
}
