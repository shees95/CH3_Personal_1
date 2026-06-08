#pragma once

#include "CoreMinimal.h"
#include "SpawnLevelTable.h"
#include "GameFramework/Actor.h"
#include "SpawnVolumn.generated.h"


class UBoxComponent;
UCLASS()
class CH3_PERSONAL_1_API ASpawnVolumn : public AActor
{
	GENERATED_BODY()

public:
	ASpawnVolumn();
protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UBoxComponent> SpawningBox;
	
	FSpawnWaveTable* SpawnWaveTable;
	
	FSpawnItemTable* GetRandomItem() const;
	
	FVector GetRandomPointInVolumn() const;
	
	
public:
	void SetSpawnWaveInfo(FSpawnWaveTable& NewSpawnWaveTable);
	AActor* SpawnRandomItem();
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	
	
};
