#include "CH3_CharacterBase.h"

#include "CH3_AttributeSet.h"
#include "CH3_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACH3_CharacterBase::ACH3_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	SkeletalMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw   = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

UAbilitySystemComponent* ACH3_CharacterBase::GetAbilitySystemComponent() const
{
	ACH3_PlayerState* PS = GetPlayerState<ACH3_PlayerState>();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

void ACH3_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACH3_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ACH3_PlayerState* PS = GetPlayerState<ACH3_PlayerState>();
	if (!PS) return;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	ASC->InitAbilityActorInfo(PS, this);
	PS->InitAbilities();
}

void ACH3_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACH3_CharacterBase::GetHealth() const
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		return ASC->GetNumericAttribute(UCH3_AttributeSet::GetHPAttribute());
	return 0.f;
}

float ACH3_CharacterBase::GetMaxHealth() const
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		return ASC->GetNumericAttribute(UCH3_AttributeSet::GetMAXHPAttribute());
	return 1.f;
}

float ACH3_CharacterBase::GetHealthPercent() const
{
	float Max = GetMaxHealth();
	return Max > 0.f ? GetHealth() / Max : 0.f;
}

void ACH3_CharacterBase::AddHealth(float Amount)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;

	const UCH3_AttributeSet* AS = ASC->GetSet<UCH3_AttributeSet>();
	if (!AS) return;

	float NewHP = FMath::Clamp(AS->GetHP() + Amount, 0.f, AS->GetMAXHP());
	const_cast<UCH3_AttributeSet*>(AS)->SetHP(NewHP);
}

void ACH3_CharacterBase::OnDeath()
{
	// 사망 애니메이션
	// 사망 UI
	// 게임 오버
}
