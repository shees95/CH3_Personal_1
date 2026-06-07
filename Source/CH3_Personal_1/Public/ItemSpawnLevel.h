#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"

USTRUCT(BlueprintType)
struct FItemSpawnLevel : public FTableROwBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LvlName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSpawnRow ItemTableRow;
	
	
};
