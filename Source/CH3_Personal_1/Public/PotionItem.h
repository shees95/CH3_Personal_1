#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "PotionItem.generated.h"

UCLASS()
class CH3_PERSONAL_1_API APotionItem : public AItemBase
{
	GENERATED_BODY()

public:
	APotionItem();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemValue;
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem() override;
};
