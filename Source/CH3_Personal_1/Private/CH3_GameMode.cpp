#include "CH3_GameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


ACH3_GameMode::ACH3_GameMode()
{
}

void ACH3_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetCurrentLevel()->GetFName() != "Lvl1") return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// 태그로 레벨의 CameraActor 자동 탐색
	if (!Camera)
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainCamera"), Found);
		if (Found.Num() > 0)
		{
			Camera = Cast<ACameraActor>(Found[0]);
		}
	}

	// 카메라 뷰로 즉시 전환
	if (Camera)
	{
		PC->SetViewTargetWithBlend(Camera, 0.0f);
	}

	// 마우스 커서 on, UI 전용 입력
	PC->bShowMouseCursor = true;
	PC->SetInputMode(FInputModeUIOnly());

	// 메인메뉴 생성
	if (MainMenuClass)
	{
		MainMenu = CreateWidget<UUserWidget>(PC, MainMenuClass);
		if (MainMenu) MainMenu->AddToViewport();
	}
}

void ACH3_GameMode::StartGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// 메뉴 제거
	if (MainMenu)
	{
		MainMenu->RemoveFromParent();
		MainMenu = nullptr;
	}

	// 캐릭터로 카메라 블렌드
	if (APawn* PlayerPawn = PC->GetPawn())
	{
		PC->SetViewTargetWithBlend(PlayerPawn, CameraBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
	}

	// 블렌드 완료 후 게임 입력 활성화
	GetWorldTimerManager().SetTimer(BlendTimerHandle, this, &ACH3_GameMode::OnBlendFinished, CameraBlendTime, false);
}

void ACH3_GameMode::OnBlendFinished()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	PC->bShowMouseCursor = false;
	PC->SetInputMode(FInputModeGameOnly());
}
