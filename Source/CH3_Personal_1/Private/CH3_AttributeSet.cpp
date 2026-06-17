#include "CH3_AttributeSet.h"

#include "CH3_CharacterBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"

UCH3_AttributeSet::UCH3_AttributeSet()
{
	InitMoveSpeed(600.f);
}

void UCH3_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UCH3_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
		if (!ASC || !ASC->AbilityActorInfo.IsValid()) return;

		ACH3_CharacterBase* Character = Cast<ACH3_CharacterBase>(ASC->AbilityActorInfo->AvatarActor.Get());
		if (Character)
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = NewValue;
			UE_LOG(LogTemp, Warning, TEXT("WalkSpeedChanged"));
		}
	}
}
