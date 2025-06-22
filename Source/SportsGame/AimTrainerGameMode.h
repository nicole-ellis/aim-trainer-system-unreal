// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetSpawner.h"
#include "GameFramework/GameModeBase.h"
#include "AimTrainerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API AAimTrainerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAimTrainerGameMode();

	virtual void BeginPlay() override;

	// Current level mode (dropping, exploding, AI moving)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EAimTrainerLevelMode> LevelModes = {
		EAimTrainerLevelMode::DropTargets,
		EAimTrainerLevelMode::ExplodingTargets,
		EAimTrainerLevelMode::AIMovingTargets
	};

	// Tracks what round you are on
	UPROPERTY(BlueprintReadWrite)
	int CurrentLevelIndex = 0;

	// To start next round
	UFUNCTION(BlueprintCallable)
	void StartNextRound();

	UFUNCTION(BlueprintCallable)
	void ShowFinalScore();

	// Reference to target spawner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	UTargetSpawner* TargetSpawner;
};


