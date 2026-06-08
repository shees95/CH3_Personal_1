#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CH3_ControllerBase.generated.h"

class UInputMappingContext;
class UInputAction;
class UCH3_InProgressWidget;
class ACH3_CharacterBase;

UCLASS()
class CH3_PERSONAL_1_API ACH3_ControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	ACH3_ControllerBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	// 입력
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Player;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Crouch;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD")
	UUserWidget* HUDWidget;
	
	
	
	// 입력 핸들러
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void DoJump(const FInputActionValue& Value);
	void DoCrouch(const FInputActionValue& Value);
	
public:
	UFUNCTION(BlueprintPure, Category="HUD")
	UUserWidget* GetHUDWidget() const { return HUDWidget; } 

};
