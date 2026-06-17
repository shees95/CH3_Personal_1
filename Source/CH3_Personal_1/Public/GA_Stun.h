#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Camera/CameraShakeBase.h"
#include "GA_Stun.generated.h"


class UInputMappingContext;

UCLASS()
class CH3_PERSONAL_1_API UGA_Stun : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Stun();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bWasCancelled,
		bool bReplicateEndAbility) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Stun")
	TSubclassOf<UGameplayEffect> StunEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Stun")
	float StunDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Stun")
	TSubclassOf<UCameraShakeBase> StunCameraShakeClass;

	TObjectPtr<UInputMappingContext> PlayerIMC;
	
private:
	void SetInputBlock(const FGameplayAbilityActorInfo* ActorInfo, bool bBlocked);

	UFUNCTION()
	void OnStunFinished();
};
