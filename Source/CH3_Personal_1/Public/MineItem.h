// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "MineItem.generated.h"

UCLASS()
class CH3_PERSONAL_1_API AMineItem : public AItemBase
{
	GENERATED_BODY()

public:
	AMineItem();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> ExplosionCollision;
	
	UPROPERTY(EditAnywhere)
	float ExplosionDelay;

	UPROPERTY(EditAnywhere)
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category="GAS")
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	FTimerHandle ExplosionTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	TObjectPtr<USoundBase> ExplodeSound;
protected:
	virtual void OnTriggered(AActor* OtherActor) override;
	
public:
	virtual void Tick(float DeltaTime) override;
	
	void MineExploder();
	
	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem() override;
};
