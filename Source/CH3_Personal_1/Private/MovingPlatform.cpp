// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Engine/TargetPoint.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 200.f;
}

void AMovingPlatform::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!FMath::IsNearlyZero(MoveSpeed)) SetProgress(DeltaTime);
	
	
}

void AMovingPlatform::SetProgress(float DeltaTime)
{
	CheckIsReverse();
	
	FVector TargetLocation = GetIsReverse() ? OriginTransform.GetLocation() : EndPoint->GetActorLocation();
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	AddActorLocalOffset(Direction * DeltaTime * MoveSpeed);
	
}

void AMovingPlatform::CheckIsReverse()
{
	if (FVector::Dist(GetActorLocation(), EndPoint->GetActorLocation()) < 5.f && !GetIsReverse()) SetIsReverse(true);
	if (FVector::Dist(GetActorLocation(), OriginTransform.GetLocation()) < 5.f && GetIsReverse()) SetIsReverse(false);
}
