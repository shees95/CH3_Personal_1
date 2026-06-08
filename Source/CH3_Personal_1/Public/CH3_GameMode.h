#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraActor.h"
#include "CH3_GameMode.generated.h"


UCLASS()
class CH3_PERSONAL_1_API ACH3_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACH3_GameMode();

	virtual void BeginPlay() override;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	ACameraActor* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraBlendTime = 1.5f;

	// Start 버튼에서 호출
	UFUNCTION(BlueprintCallable, Category="Game")
	void StartGame();

private:
	UPROPERTY()
	UUserWidget* MainMenu;

	FTimerHandle BlendTimerHandle;

	void OnBlendFinished();
};
