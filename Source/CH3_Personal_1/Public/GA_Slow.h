#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Camera/CameraShakeBase.h"
#include "GA_Slow.generated.h"

UCLASS()
class CH3_PERSONAL_1_API UGA_Slow : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Slow();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bWasCancelled,
		bool bReplicateEndAbility) override;

	UPROPERTY(EditDefaultsOnly, Category="Slow")
	TSubclassOf<UGameplayEffect> SlowEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Slow")
	float SlowDuration = 3.f;

	UPROPERTY(EditDefaultsOnly, Category="Slow")
	float SlowMagnitude = 200.f;

	UPROPERTY(EditDefaultsOnly, Category="Slow")
	TSubclassOf<UCameraShakeBase> SlowCameraShakeClass;

private:
	UFUNCTION()
	void OnSlowFinished();
};
