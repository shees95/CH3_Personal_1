#include "PotionItem.h"

#include "AbilitySystemComponent.h"
#include "CH3_CharacterBase.h"

APotionItem::APotionItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemType = "DefaultPotion";
}

void APotionItem::BeginPlay()
{
	Super::BeginPlay();
}

void APotionItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APotionItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (!Activator || !Activator->ActorHasTag("Player")) return;

	ACH3_CharacterBase* Character = Cast<ACH3_CharacterBase>(Activator);
	if (!Character) return;

	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!ASC || !HealEffect) return;

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(HealEffect, 1.f, Context);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void APotionItem::DestroyItem()
{
	Super::DestroyItem();
}
