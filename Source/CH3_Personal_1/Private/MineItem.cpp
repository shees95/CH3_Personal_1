#include "MineItem.h"

#include "AbilitySystemComponent.h"
#include "CH3_CharacterBase.h"
#include "Components/SphereComponent.h"

AMineItem::AMineItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ExplosionDelay = 3.f;
	ExplosionRadius = 300.f;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetSphereRadius(ExplosionRadius);
	ExplosionCollision->SetupAttachment(Root);

	ItemType = "Mine";
}

void AMineItem::BeginPlay()
{
	Super::BeginPlay();
}

void AMineItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMineItem::MineExploder()
{
	SetState(EItemState::OnActivated, this);
}

void AMineItem::OnTriggered(AActor* OtherActor)
{
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::MineExploder, ExplosionDelay, false);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Mine has explosion"));

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || !Actor->ActorHasTag("player")) continue;

		ACH3_CharacterBase* Character = Cast<ACH3_CharacterBase>(Actor);
		if (!Character) continue;

		UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
		if (!ASC) continue;

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("%s has hitten"), *Actor->GetName()));

		if (DamageEffect)
		{
			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			Context.AddSourceObject(this);
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(DamageEffect, 1.f, Context);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}

		FGameplayTagContainer SlowTag;
		SlowTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Slow")));
		ASC->TryActivateAbilitiesByTag(SlowTag);
	}

	SetState(EItemState::OnDestroyed, nullptr);
}

void AMineItem::DestroyItem()
{
	Super::DestroyItem();
}
