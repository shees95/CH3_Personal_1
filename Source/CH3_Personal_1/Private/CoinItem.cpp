#include "CoinItem.h"



ACoinItem::ACoinItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ItemValue = 35;
	ItemType = "DefaultCoin";
}

void ACoinItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoinItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Player Gain Coin")));
	}
}

void ACoinItem::DestroyItem()
{
	Super::DestroyItem();
}
