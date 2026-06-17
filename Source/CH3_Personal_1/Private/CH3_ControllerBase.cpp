#include "CH3_ControllerBase.h"

#include "AbilitySystemComponent.h"
#include "CH3_CharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"

ACH3_ControllerBase::ACH3_ControllerBase()
: IMC_Player(nullptr), IA_Move(nullptr), IA_Look(nullptr)
	,IA_Jump(nullptr), IA_Crouch(nullptr), HUDWidgetClass(nullptr), HUDWidget(nullptr)
{
}

void ACH3_ControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	// IMC 등록
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Player) Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}
	
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
		
	}
}

void ACH3_ControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EIC) return;

	if (IA_Move)   EIC->BindAction(IA_Move,   ETriggerEvent::Triggered, this, &ACH3_ControllerBase::Move);
	if (IA_Look)   EIC->BindAction(IA_Look,   ETriggerEvent::Triggered, this, &ACH3_ControllerBase::Look);
	if (IA_Jump)   EIC->BindAction(IA_Jump,   ETriggerEvent::Started,   this, &ACH3_ControllerBase::DoJump);
	if (IA_Crouch) EIC->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &ACH3_ControllerBase::DoCrouch);
}

void ACH3_ControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

// ── 입력 ─────────────────────────────────────────────────────────

void ACH3_ControllerBase::Move(const FInputActionValue& Value)
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn) return;
	
	const FVector2D MoveValue = Value.Get<FVector2D>();
	if (MoveValue.IsNearlyZero()) return;

	const FVector Forward = MyPawn->GetActorForwardVector();
	const FVector Right   = -MyPawn->GetActorRightVector();

	MyPawn->AddMovementInput(Forward, MoveValue.X);
	MyPawn->AddMovementInput(Right,   MoveValue.Y);
}

void ACH3_ControllerBase::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.IsNearlyZero()) return;

	AddYawInput(LookValue.X);
	AddPitchInput(-LookValue.Y);
}

void ACH3_ControllerBase::DoJump(const FInputActionValue& Value)
{
	if (ACharacter* Char = Cast<ACharacter>(GetPawn()))
	{
		Char->Jump();
	}
}

void ACH3_ControllerBase::DoCrouch(const FInputActionValue& Value)
{
	ACharacter* Char = Cast<ACharacter>(GetPawn());
	if (!Char) return;

	const bool bPressed = Value.Get<bool>();
	bPressed ? Char->Crouch() : Char->UnCrouch();
}
