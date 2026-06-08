#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "CoinItem.generated.h"

UCLASS()
class CH3_PERSONAL_1_API ACoinItem : public AItemBase
{
	GENERATED_BODY()

public:
	ACoinItem();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemValue;
	
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
	
	int32 GetItemValue() const { return ItemValue; }
	
	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem() override;
};
