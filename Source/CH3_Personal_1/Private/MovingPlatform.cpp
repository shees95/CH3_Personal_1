#include "MovingPlatform.h"
#include "Engine/TargetPoint.h"
#include "CH3_CharacterBase.h"
#include "AbilitySystemComponent.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 200.f;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

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
	AddActorLocalOffset(Direction * DeltaTime * MoveSpeed, true);
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

void AMovingPlatform::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (!StunAbilityClass) return;

	ACH3_CharacterBase* Character = Cast<ACH3_CharacterBase>(Other);
	if (!Character) return;

	UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent();
	if (!TargetASC) return;

	FGameplayTagContainer AbilityTag;
	AbilityTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Stun")));
	TargetASC->TryActivateAbilitiesByTag(AbilityTag);
}
