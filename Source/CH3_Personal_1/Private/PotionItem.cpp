#include "PotionItem.h"

#include "CH3_CharacterBase.h"


APotionItem::APotionItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ItemValue = 50;
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
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ACH3_CharacterBase* PlayerCharacter = Cast<ACH3_CharacterBase>(Activator))
		{
			PlayerCharacter->AddHealth(ItemValue);
		}
	}
}

void APotionItem::DestroyItem()
{
	Super::DestroyItem();
}

