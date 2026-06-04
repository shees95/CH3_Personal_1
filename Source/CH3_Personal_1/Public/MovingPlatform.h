// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "MovingPlatform.generated.h"

class ATargetPoint;

#define 틱 true

UCLASS()
class CH3_PERSONAL_1_API AMovingPlatform : public APlatformBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	ATargetPoint* EndPoint;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 200.f;
	
	FTimerHandle PlatformMoveHandle;
	
	UPROPERTY(EditAnywhere)
	bool RandomPlatformMove = true;
	
	UPROPERTY(EditAnywhere)
	float TimeToPlatformMove = 15.f;
	
	bool IsReverse = false;
public:	
	AMovingPlatform();
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetProgress(float DeltaTime);
	void CheckIsReverse();
	void SetIsReverse(bool _InIsReverse) { IsReverse = _InIsReverse; }
	bool GetIsReverse() { return IsReverse; }
	void MoveEndPoint();
};
