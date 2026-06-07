#include "PotionItem.h"



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
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Player has Healed")));
	}
}

void APotionItem::DestroyItem()
{
	Super::DestroyItem();
}

