#include "SpawnVolumn.h"
#include "Components/BoxComponent.h"


ASpawnVolumn::ASpawnVolumn()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	SetRootComponent(Root);
	
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Root);
	
	ItemDataTable = nullptr;
}

void ASpawnVolumn::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i=0;i<10;i++)
	{
		SpawnRandomItem();
	}
}

FVector ASpawnVolumn::GetRandomPointInVolumn() const
{
	// 반지름
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	// 중앙
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(FMath::FRandRange(-BoxExtent.X, BoxExtent.X), FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y), FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}

void ASpawnVolumn::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			SpawnItem(ActualClass);
		}
	}
}

FItemSpawnRow* ASpawnVolumn::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;
	
	TArray<FItemSpawnRow*> AllItemRows;
	
	static const FString ContextString(TEXT("ItemSpawnContextError"));
	ItemDataTable->GetAllRows(ContextString, AllItemRows);
	
	if (AllItemRows.IsEmpty()) return nullptr;
	
	float TotalChance = 0.f;
	
	for (const FItemSpawnRow* Row : AllItemRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}
	
	const float RandValue = FMath::FRandRange(0.f, TotalChance);
	float AccumulatedChance = 0.f;
	
	for (FItemSpawnRow* Row : AllItemRows)
	{
		AccumulatedChance += Row->SpawnChance;
		
		if (RandValue <= AccumulatedChance)
		{
			return Row;
		}
	}
	
	return nullptr;
}

void ASpawnVolumn::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return;
	
	GetWorld()->SpawnActor<AActor>(ItemClass, GetRandomPointInVolumn(), FRotator::ZeroRotator);
}
