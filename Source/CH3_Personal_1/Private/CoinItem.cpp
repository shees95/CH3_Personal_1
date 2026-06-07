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
				GameState->AddScore(ItemValue);
				GameState->OnCoinCollected();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Score : %d"), GameState->GetScore()));
			}
		}
	}
}

void ACoinItem::DestroyItem()
{
	Super::DestroyItem();
}
