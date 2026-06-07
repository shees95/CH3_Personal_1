#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"



UINTERFACE()
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class CH3_PERSONAL_1_API IItemInterface
{
	GENERATED_BODY()
	
protected:

public:
	UFUNCTION()
	virtual void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
	UFUNCTION()
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) = 0;
	
	virtual void ActivateItem(AActor* Activator) = 0;
	virtual void DestroyItem() = 0;
	
};
