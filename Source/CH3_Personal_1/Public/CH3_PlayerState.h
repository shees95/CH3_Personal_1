#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerState.h"
#include "CH3_PlayerState.generated.h"

UCLASS()
class CH3_PERSONAL_1_API ACH3_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACH3_PlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

	void InitAbilities();

	UPROPERTY(EditDefaultsOnly, Category="ASC")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

private:
	UPROPERTY(VisibleAnywhere, Category="ASC")
	TObjectPtr<UAbilitySystemComponent> ASC;
};
