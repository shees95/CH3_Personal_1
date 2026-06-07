#include "MineItem.h"

#include "Components/SphereComponent.h"


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
	}
	
	
	SetState(EItemState::OnDestroyed, nullptr);
}

void AMineItem::DestroyItem()
{
	Super::DestroyItem();
}

