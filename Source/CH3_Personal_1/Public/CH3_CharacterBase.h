#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CH3_CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CH3_PERSONAL_1_API ACH3_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACH3_CharacterBase();
	
protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 100.f;

	void OnDeath();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const { return MaxHealth > 0.f ? Health / MaxHealth : 0.f; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
};
