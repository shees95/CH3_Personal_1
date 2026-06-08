// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"
#include "CH3_PawnBase.generated.h"

class UInputMappingContext;
class UCameraComponent;
class UCapsuleComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class UInputAction;

UCLASS()
class CH3_PERSONAL_1_API ACH3_PawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACH3_PawnBase();
private:
	// 컴포넌트
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	
	// IA
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Look;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_GoUp;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_GoDown;
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Player;
	
	// 이동 함수
	UPROPERTY(EditAnywhere)
	float OriginMoveSpeed = 350.f;
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere)
	float OriginUpSpeed = 98.f;
	
	float UpSpeed;
	
	UPROPERTY(EditAnywhere)
	float OriginDownSpeed = 98.f;
	
	float DownSpeed;
	
	float MaxZVelocity = 1200.f;
	
	UPROPERTY(VisibleAnywhere)
	float ZVelocity = 0.f;
	
	UPROPERTY(EditAnywhere)
	float StepUpHeight = 90.f;
	
	UPROPERTY(EditAnywhere)
	float LookSensitivity_X = 600.f;
	UPROPERTY(EditAnywhere)
	float LookSensitivity_Y = 400.f;
	
	// 인공 중력
	UPROPERTY(EditAnywhere)
	float Gravity = 980.f;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	bool bIsJump = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	bool bIsInAir = false;
	
	FTimerHandle CheckInAirTimerHandle;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void GoUp(const FInputActionValue& Value);
	void GoDown(const FInputActionValue& Value);

	
	
	void ApplyGravity(float DeltaTime);
	void CheckInAir();
	void Land(FVector ImpactPoint);
	
	bool HandleSlopeMovement(const FVector& MoveDirection, float DeltaTime);
};
