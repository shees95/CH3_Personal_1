#include "CH3_CharacterBase.h"

#include "CH3_GameState.h"
#include "CH3_PlayerState.h"
#include "CH3_AttributeSet.h"
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

	MaxHealth = 100.f;
	Health    = MaxHealth;
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

	ASC->GetGameplayAttributeValueChangeDelegate(UCH3_AttributeSet::GetMoveSpeedAttribute())
		.AddUObject(this, &ACH3_CharacterBase::OnMoveSpeedChanged);
}

void ACH3_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ACH3_CharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health : %.1f / %.1f"), Health, MaxHealth);

	OnHealthChanged.Broadcast(GetHealthPercent());
	Cast<ACH3_GameState>(GetWorld()->GetGameState())->UpdateHUD();

	if (Health <= 0.f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ACH3_CharacterBase::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health : %.1f / %.1f"), Health, MaxHealth);

	OnHealthChanged.Broadcast(GetHealthPercent());
	Cast<ACH3_GameState>(GetWorld()->GetGameState())->UpdateHUD();
}

void ACH3_CharacterBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void ACH3_CharacterBase::OnDeath()
{
	// 사망 애니메이션
	// 사망 UI
	// 게임 오버
}
