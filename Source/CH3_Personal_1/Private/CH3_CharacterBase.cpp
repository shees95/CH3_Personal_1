#include "CH3_CharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACH3_CharacterBase::ACH3_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	SkeletalMesh->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;  // 스프링암이 컨트롤러 회전 따라감

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 캐릭터가 컨트롤러 회전을 직접 따라가지 않음
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	
	
	// 이동 방향으로 캐릭터 메시 자동 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACH3_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;

	Subsystem->AddMappingContext(IMC_Player, 0);
}

// Called every frame
void ACH3_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACH3_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* IMC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (IMC)
	{
		if (IA_Move) IMC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACH3_CharacterBase::DoMove);
		if (IA_Look) IMC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACH3_CharacterBase::DoLook);
		if (IA_Jump) IMC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACH3_CharacterBase::DoJump);
		if (IA_Crouch) IMC->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &ACH3_CharacterBase::DoCrouch);
		
	}
}


void ACH3_CharacterBase::DoMove(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MoveValue = Value.Get<FVector2D>();
	if (MoveValue.IsNearlyZero()) return;

	const FVector Forward = GetActorForwardVector();
	const FVector Right   = -GetActorRightVector();

	AddMovementInput(Forward, MoveValue.X);
	AddMovementInput(Right,   MoveValue.Y);
}

void ACH3_CharacterBase::DoLook(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.IsNearlyZero()) return;

	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(-LookValue.Y);
}


void ACH3_CharacterBase::DoJump(const FInputActionValue& Value)
{
	Jump();
}

void ACH3_CharacterBase::DoCrouch(const FInputActionValue& Value)
{
	const bool bPressed = Value.Get<bool>();
	if (bPressed) Crouch();
	else          UnCrouch();
}

