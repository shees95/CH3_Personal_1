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

	if (HandleSlopeMovement(MoveDirection, GetWorld()->GetDeltaSeconds()))
	{
		return;
	}
	
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
	if (ZVelocity > 0.f)
	{
		bIsInAir = true;
		MoveSpeed = OriginMoveSpeed / 2.f;
		return;
	}

	FHitResult HitResult_L;
	FHitResult HitResult_R;

	const FVector Start_L = Mesh->GetSocketLocation(TEXT("Foot_L"));
	const FVector End_L = Start_L + FVector(0.f, 0.f, -1000.f);

	const FVector Start_R = Mesh->GetSocketLocation(TEXT("Foot_R"));
	const FVector End_R = Start_R + FVector(0.f, 0.f, -1000.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit_L = GetWorld()->LineTraceSingleByChannel(HitResult_L,Start_L,End_L,ECC_Visibility,Params);

	const bool bHit_R = GetWorld()->LineTraceSingleByChannel(HitResult_R,Start_R,End_R,ECC_Visibility,Params);

	if (!bHit_L && !bHit_R)
	{
		MoveSpeed = OriginMoveSpeed / 2.f;
		bIsInAir = true;
		return;
	}

	FHitResult BestHit;

	if (bHit_L && bHit_R)
	{
		BestHit = HitResult_L.Distance <= HitResult_R.Distance ? HitResult_L : HitResult_R;
	}
	else
	{
		BestHit = bHit_L ? HitResult_L : HitResult_R;
	}

	const float GroundDistance = BestHit.Distance - StepUpHeight;

	if (GroundDistance <= 5.f)
	{
		Land(BestHit.ImpactPoint);
	}
	else
	{
		MoveSpeed = OriginMoveSpeed / 2.f;
		bIsInAir = true;
	}
}

void ACH3_PawnBase::Land(FVector ImpactPoint)
{
	ZVelocity = 0.f;
	bIsInAir = false;
	bIsJump = false;
	MoveSpeed = OriginMoveSpeed;

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, ImpactPoint.Z + StepUpHeight + 2.f));
}

bool ACH3_PawnBase::HandleSlopeMovement(const FVector& MoveDirection, float DeltaTime)
{
	if (MoveDirection.IsNearlyZero()) return false;
	if (bIsInAir) return false;
	
	const float TraceStartHeight = 80.f;
	const float TraceLength = 200.f;
	const float MaxSlopeAngle = 45.f;
	
	const FVector MoveOffset = MoveDirection.GetSafeNormal() * MoveSpeed * DeltaTime;
	
	FVector TraceStart = GetActorLocation() + MoveOffset;
	TraceStart.Z += TraceStartHeight;
	
	const FVector TraceEnd = TraceStart + FVector(0.f, 0.f, -TraceLength);
	
	FHitResult HitResult;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 0.05f, 0, 2.f);
	
	if (!bHit) return false;
	
	const float SlopeAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(HitResult.ImpactNormal, FVector::UpVector)));
	
	if (SlopeAngle > MaxSlopeAngle) return false;
	
	FVector NewLocation = GetActorLocation() + MoveOffset;
	NewLocation.Z = HitResult.ImpactPoint.Z + StepUpHeight + 2.f;
	
	SetActorLocation(NewLocation, true);
	
	return true;
}
