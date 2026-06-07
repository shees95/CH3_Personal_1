// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CH3_CharacterBase.generated.h"

class UInputMappingContext;
class UInputAction;

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CH3_PERSONAL_1_API ACH3_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACH3_CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Look;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Crouch;
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* IMC_Player;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DoMove(const FInputActionValue& Value);
	void DoLook(const FInputActionValue& Value);
	void DoJump(const FInputActionValue& Value);
	void DoCrouch(const FInputActionValue& Value);
	
};
