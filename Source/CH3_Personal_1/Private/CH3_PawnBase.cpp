// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3_PawnBase.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACH3_PawnBase::ACH3_PawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	SetRootComponent(Capsule);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetupAttachment(Capsule);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	Mesh->SetSimulatePhysics(false);
	
	
	MoveSpeed = OriginMoveSpeed;
	UpSpeed = OriginUpSpeed;
	DownSpeed = OriginDownSpeed;
}

// Called when the game starts or when spawned
void ACH3_PawnBase::BeginPlay()
{
	Super::BeginPlay();
	
	
	// 컨트롤러 연결
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	
	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;
	
	Subsystem->AddMappingContext(IMC_Player, 0);
}

// Called every frame
void ACH3_PawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 중력 및 낙하 체크
	ApplyGravity(DeltaTime);
	CheckInAir();
	
	
	
}



// Called to bind functionality to input
void ACH3_PawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* IMC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (IMC)
	{
		if (IA_Move) IMC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACH3_PawnBase::Move);
		if (IA_Look) IMC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACH3_PawnBase::Look);
		if (IA_GoUp) IMC->BindAction(IA_GoUp, ETriggerEvent::Triggered, this, &ACH3_PawnBase::GoUp);
		if (IA_GoDown) IMC->BindAction(IA_GoDown, ETriggerEvent::Triggered, this, &ACH3_PawnBase::GoDown);
		
	}
	
}

void ACH3_PawnBase::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	if (MoveValue.IsNearlyZero()) return;
	

	const FVector Forward = Mesh->GetForwardVector();
	const FVector Right = Mesh->GetRightVector();

	const FVector MoveDirection = (Forward * MoveValue.Y) + (Right * MoveValue.X);

	AddActorWorldOffset(MoveDirection.GetSafeNormal() * MoveSpeed * GetWorld()->GetDeltaSeconds(),true);
}

void ACH3_PawnBase::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.IsNearlyZero()) return;
	
	// 스프링암 돌리기
	SpringArm->AddLocalRotation(FRotator(LookValue.Y * LookSensitivity_Y * GetWorld()->GetDeltaSeconds(),LookValue.X * LookSensitivity_X * GetWorld()->GetDeltaSeconds(),  0));
	SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch,SpringArm->GetRelativeRotation().Yaw,0));
	// 캐릭터 카메라 방향 따라가기
	Mesh->SetWorldRotation(FRotator(0, SpringArm->GetComponentRotation().Yaw - 90, 0));
}

void ACH3_PawnBase::GoUp(const FInputActionValue& Value)
{
	const bool UpValue = Value.Get<bool>();
	if (!UpValue) return;
	
	ZVelocity = UpSpeed * 5;
	
	bIsJump = true;
	bIsInAir = true;
	
	// ZVelocity = FMath::Clamp(ZVelocity + UpSpeed * GetWorld()->GetDeltaSeconds(), -MaxZVelocity, MaxZVelocity);

}

void ACH3_PawnBase::GoDown(const FInputActionValue& Value)
{
	const bool DownValue = Value.Get<bool>();
	if (!DownValue) return;
	
	bIsInAir = true;
	
	// ZVelocity = FMath::Clamp(ZVelocity - DownSpeed * GetWorld()->GetDeltaSeconds(), -MaxZVelocity, MaxZVelocity);
}

void ACH3_PawnBase::ApplyGravity(float DeltaTime)
{
	if (!bIsInAir) return;
	
	ZVelocity = FMath::Clamp(ZVelocity - Gravity * DeltaTime, -MaxZVelocity, MaxZVelocity);
	
	UE_LOG(LogTemp, Warning, TEXT("ZVelocity : %f"), ZVelocity);
	
	AddActorWorldOffset(FVector(0.f, 0.f, ZVelocity * DeltaTime), true);
}

void ACH3_PawnBase::CheckInAir()
{
	// 오르는 중
	if (ZVelocity > 0.f)
	{
		bIsInAir = true;
		MoveSpeed = OriginMoveSpeed / 2.f;
		return;
	}
	
	FHitResult HitResult;
	
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0.f,0.f,-1000.f);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	// 바닥 없음
	if (!bHit)
	{
		MoveSpeed = OriginMoveSpeed / 2.f;
		bIsInAir = true;
		return;
	}
	
	const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	const float GroundDistance = HitResult.Distance - HalfHeight;
	
	if (GroundDistance <= 1.f)
	{
		Land(HitResult.ImpactPoint);
	}
	else
	{
		MoveSpeed = OriginMoveSpeed / 2.f;
		bIsInAir = true;
		return;
	}
	
}

void ACH3_PawnBase::Land(FVector ImpactPoint)
{
	ZVelocity = 0.f;
	bIsInAir = false;
	bIsJump = false;
	MoveSpeed = OriginMoveSpeed;
	
	const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	const FVector CurrentLocation = GetActorLocation();
	
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ImpactPoint.Z + HalfHeight + 1.f));
	
}