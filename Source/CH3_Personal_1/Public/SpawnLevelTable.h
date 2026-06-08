#pragma once

#include "CoreMinimal.h"
#include "SpawnLevelTable.generated.h"

USTRUCT(BlueprintType)
struct FSpawnItemTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};

USTRUCT(BlueprintType)
struct FSpawnWaveTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnItemTable> ItemList;
};

USTRUCT(BlueprintType)
struct FSpawnLevelTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LvlName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnWaveTable> Waves;
};
