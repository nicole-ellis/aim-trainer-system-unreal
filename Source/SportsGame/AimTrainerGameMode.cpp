// Fill out your copyright notice in the Description page of Project Settings.


#include "AimTrainerGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "AimTrainingCharacter.h"

AAimTrainerGameMode::AAimTrainerGameMode()
{
	// Set default pawn class to aim training character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_AimTrainingCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAimTrainerGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	
}

void AAimTrainerGameMode::StartNextRound()
{
	if (!TargetSpawner) return;

	if (CurrentLevelIndex < LevelModes.Num())
	{
		TargetSpawner->LevelMode = LevelModes[CurrentLevelIndex];
		TargetSpawner->BeginTraining();
		CurrentLevelIndex++;
	}
	else
	{
		// Show leaderboard/final screen
		ShowFinalScore();
	}
}

void AAimTrainerGameMode::ShowFinalScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Final Score: Show Leaderboard or summary screen here"))
	// Replace with actual logic
}
