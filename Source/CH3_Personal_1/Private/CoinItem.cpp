#include "CoinItem.h"

#include "CH3_GameState.h"
#include "Engine/World.h"



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
		if (UWorld* World = GetWorld())
		{
			if (ACH3_GameState* GameState = World->GetGameState<ACH3_GameState>())
			{
				GameState->OnCoinCollected(this);
			}
		}
	}
}

void ACoinItem::DestroyItem()
{
	Super::DestroyItem();
}
