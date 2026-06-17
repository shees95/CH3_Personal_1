#include "CH3_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "CH3_AttributeSet.h"

ACH3_PlayerState::ACH3_PlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CreateDefaultSubobject<UCH3_AttributeSet>(TEXT("AttributeSet"));
}

void ACH3_PlayerState::InitAbilities()
{
	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass) continue;
		ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
	git 
}
