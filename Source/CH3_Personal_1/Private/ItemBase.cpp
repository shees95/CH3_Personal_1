#include "ItemBase.h"
#include "Components/SphereComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Root);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(Collision);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnItemBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnItemEndOverlap);
	
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::ActivateItem(AActor* Activator)
{
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Activate ItemBase")));
}


void AItemBase::DestroyItem()
{
	Destroy();
}

void AItemBase::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("overlapping")));
		SetState(EItemState::OnTriggered, OtherActor);
		CollisionActor = OtherActor;
	}
}

void AItemBase::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CollisionActor = nullptr;
}



void AItemBase::SetState(EItemState NewState, AActor* OtherActor = nullptr)
{
	if (CurrentState == NewState) return;

	switch (NewState)
	{
	case EItemState::None:
		CurrentState = NewState;
		DestroyItem();
		break;
		
	case EItemState::OnSpawned:
		CurrentState = NewState;
		OnSpawned();
		break;
		
	case EItemState::OnIdle:
		CurrentState = NewState;
		OnIdle();
		break;
		
	case EItemState::OnTriggered:
		CurrentState = NewState;
		OnTriggered(OtherActor);
		break;
		
	case EItemState::OnActivated:
		CurrentState = NewState;
		OnActivated(OtherActor);
		break;
		
	case EItemState::OnDestroyed:
		CurrentState = NewState;
		OnDestroyed();
		break;
		
	default:
		break;
	}
}

void AItemBase::OnSpawned()
{
	SetState(EItemState::OnIdle, nullptr);
}
void AItemBase::OnTriggered(AActor* OtherActor)
{
	SetState(EItemState::OnActivated, OtherActor);
}
void AItemBase::OnActivated(AActor* OtherActor)
{
	ActivateItem(OtherActor);
	SetState(EItemState::OnDestroyed, nullptr);
}
void AItemBase::OnDestroyed()
{
	SetState(EItemState::None, nullptr);
	DestroyItem();
}