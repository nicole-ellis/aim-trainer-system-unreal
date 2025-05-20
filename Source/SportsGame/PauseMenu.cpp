// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "SportsGameGameInstance.h"
#include "SportsGameSaveGame.h"
#include "SportsGameGameMode.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ContinueButton->OnClicked.AddDynamic(this, &UPauseMenu::ContinueButtonClick);
	SaveButton->OnClicked.AddDynamic(this, &UPauseMenu::SaveButtonClick);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitButtonClick);
}

void UPauseMenu::ContinueButtonClick()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
 	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
 	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
 	RemoveFromParent();
}

void UPauseMenu::SaveButtonClick()
{
	USportsGameSaveGame* SaveGameInstance = Cast<USportsGameSaveGame>(UGameplayStatics::CreateSaveGameObject(USportsGameSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		ASportsGameGameMode* SportsGameGameMode = Cast<ASportsGameGameMode>(GetWorld()->GetAuthGameMode());
		if (SportsGameGameMode)
		{
			SportsGameGameMode->Save(SaveGameInstance);
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, "TestSave", 0);
		}
	}
}

void UPauseMenu::QuitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
