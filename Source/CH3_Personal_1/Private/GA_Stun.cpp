#include "GA_Stun.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "CH3_ControllerBase.h"
#include "EnhancedInputSubsystems.h"

UGA_Stun::UGA_Stun()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Stun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (StunEffectClass)
	{
		SetInputBlock(ActorInfo, true);
		
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(StunEffectClass, 1.f, Context);
		if (Spec.IsValid())
		{
			Spec.Data->SetByCallerTagMagnitudes.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Duration")), StunDuration);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	if (StunCameraShakeClass)
	{
		APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
		if (PC) PC->ClientStartCameraShake(StunCameraShakeClass);
	}

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, StunDuration);
	WaitTask->OnFinish.AddDynamic(this, &UGA_Stun::OnStunFinished);
	WaitTask->ReadyForActivation();
}

void UGA_Stun::OnStunFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Stun::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bWasCancelled, bool bReplicateEndAbility)
{
	SetInputBlock(ActorInfo, false);

	if (StunCameraShakeClass)
	{
		APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
		if (PC) PC->ClientStopCameraShake(StunCameraShakeClass);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bWasCancelled, bReplicateEndAbility);
}

void UGA_Stun::SetInputBlock(const FGameplayAbilityActorInfo* ActorInfo, bool bBlocked)
{
	if (!ActorInfo) return;
	
	ACH3_ControllerBase* PC = Cast<ACH3_ControllerBase>(ActorInfo->PlayerController.Get());
	if (!PC) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem) return;
	
	if (!PlayerIMC) PlayerIMC = PC->GetIMC();
	
	if (bBlocked)
		Subsystem->RemoveMappingContext(PlayerIMC);
	else
		Subsystem->AddMappingContext(PlayerIMC, 0);
}
