#include "GA_Slow.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UGA_Slow::UGA_Slow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Slow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (SlowEffectClass)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(SlowEffectClass, 1.f, Context);
		if (Spec.IsValid())
		{
			Spec.Data->SetByCallerTagMagnitudes.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Duration")), SlowDuration);
			Spec.Data->SetByCallerTagMagnitudes.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Magnitude")), SlowMagnitude);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	if (SlowCameraShakeClass)
	{
		APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
		if (PC) PC->ClientStartCameraShake(SlowCameraShakeClass);
	}

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, SlowDuration);
	WaitTask->OnFinish.AddDynamic(this, &UGA_Slow::OnSlowFinished);
	WaitTask->ReadyForActivation();
}

void UGA_Slow::OnSlowFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Slow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bWasCancelled, bool bReplicateEndAbility)
{
	if (SlowCameraShakeClass)
	{
		APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
		if (PC) PC->ClientStopCameraShake(SlowCameraShakeClass);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bWasCancelled, bReplicateEndAbility);
}
