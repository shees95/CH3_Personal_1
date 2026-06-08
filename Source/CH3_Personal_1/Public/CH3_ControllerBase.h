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

	// 입력 핸들러
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void DoJump(const FInputActionValue& Value);
	void DoCrouch(const FInputActionValue& Value);

	// HP 변경 콜백 (CharacterBase 델리게이트에 바인딩)
	UFUNCTION()
	void OnHealthChanged(float Percent);

public:
	// GameState 등 외부에서 진행도 갱신 시 호출
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateProgress(float Percent);

	// HP UI 직접 갱신 (필요 시 외부 호출 가능)
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHP(float Percent);
};
