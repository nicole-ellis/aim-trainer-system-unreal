// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		UUserWidget* MainMenu = Cast<UUserWidget>(CreateWidget(GetGameInstance(), MainMenuClass));
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;
		}
		MainMenu->AddToViewport();
	}
}
