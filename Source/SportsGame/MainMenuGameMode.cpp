// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/UserWidget.h"
#include "MainMenuGameMode.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		UUserWidget* MainMenu = Cast<UUserWidget>(CreateWidget(GetGameInstance(), MainMenuClass));
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		MainMenu->AddToViewport();
	}
}
