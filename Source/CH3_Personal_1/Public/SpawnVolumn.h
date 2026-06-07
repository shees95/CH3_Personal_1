#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<AActor>> ItemList;
	
	
	FItemSpawnRow* GetRandomItem() const;
	
	FVector GetRandomPointInVolumn() const;
	
	
public:
	AActor* SpawnRandomItem();
	
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	
	
};
