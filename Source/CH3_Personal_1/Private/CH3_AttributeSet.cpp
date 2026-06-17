#include "CH3_AttributeSet.h"

#include "CH3_CharacterBase.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"

UCH3_AttributeSet::UCH3_AttributeSet()
{
	InitMAXHP(100.f);
	InitHP(100.f);
	InitMoveSpeed(600.f);
}

void UCH3_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.f, GetMAXHP()));
	}
}

void UCH3_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	if (!ASC || !ASC->AbilityActorInfo.IsValid()) return;

	ACH3_CharacterBase* Character = Cast<ACH3_CharacterBase>(ASC->AbilityActorInfo->AvatarActor.Get());
	if (!Character) return;

	if (Attribute == GetHPAttribute())
	{
		float Percent = GetMAXHP() > 0.f ? NewValue / GetMAXHP() : 0.f;
		Character->OnHealthChanged.Broadcast(Percent);

		if (NewValue <= 0.f)
		{
			Character->OnDeath();
		}
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = NewValue;
	}
}
