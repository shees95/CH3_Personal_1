// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class USphereComponent;

UENUM(Blueprinttype)
enum class EItemState : uint8
{
	None,
	OnSpawned,
	OnIdle,
	OnTriggered,
	OnActivated,
	OnDestroyed
};


UCLASS()
class CH3_PERSONAL_1_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	AItemBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> Collision;
	
	
	UPROPERTY()
	FName ItemType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemState CurrentState = EItemState::None;
	
	UPROPERTY()
	TObjectPtr<AActor> CollisionActor;
	
	
protected:
	virtual void BeginPlay() override;
	
	
	virtual void OnSpawned();
	virtual void OnIdle() {};
	virtual void OnTriggered(AActor* OtherActor);
	virtual void OnActivated(AActor* OtherActor);
	virtual void OnDestroyed();
	
public:
	void SetState(EItemState NewState, AActor* OtherActor);
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem() override;
	
	virtual void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
