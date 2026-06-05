// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Engine/TargetPoint.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 200.f;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	// 목적지 랜덤 이동
	GetWorld()->GetTimerManager().SetTimer(PlatformMoveHandle, this, &AMovingPlatform::MoveEndPoint, TimeToPlatformMove);
	
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetProgress(DeltaTime);
	
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

void AMovingPlatform::MoveEndPoint()
{
	EndPoint->AddActorLocalOffset(FVector(FMath::RandRange(-80, 80), FMath::RandRange(-80, 80), 0));
	
	GetWorld()->GetTimerManager().SetTimer(PlatformMoveHandle, this, &AMovingPlatform::MoveEndPoint, TimeToPlatformMove);
}