#include "MineItem.h"

#include "AbilitySystemComponent.h"
#include "CH3_CharacterBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AMineItem::AMineItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	ExplosionDelay = 3.f;
	ExplosionDamage = 30.f;
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
	
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Mine has explosion")));
	
	for (AActor* Actor : OverlappingActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("%s has hitten"), *Actor->GetName()));
		if (Actor && Actor->ActorHasTag("player"))
		{
			UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());

			if (ACH3_CharacterBase* Character = Cast<ACH3_CharacterBase>(Actor))
			{
				if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
				{
					FGameplayTagContainer Tag;
					Tag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Slow")));
					ASC->TryActivateAbilitiesByTag(Tag);
				}
			}
		}
		
	}
	
	
	SetState(EItemState::OnDestroyed, nullptr);
}

void AMineItem::DestroyItem()
{
	Super::DestroyItem();
}

