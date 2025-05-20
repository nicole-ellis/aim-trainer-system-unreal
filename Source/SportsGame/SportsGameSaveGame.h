// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SportsGameSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API USportsGameSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString SaveSlotName;
	UPROPERTY()
		int UserIndex;

	UPROPERTY()
		FVector PlayerPosition;
	UPROPERTY()
		FRotator PlayerRotation;
	UPROPERTY()
		float PlayerCurrentStamina;
	UPROPERTY()
		float PlayerMaxStamina;
	UPROPERTY()
		float PlayerCurrentEXP;
	UPROPERTY()
		float PlayerEXPToLevel;
	UPROPERTY()
		float PlayerKickPower;

	UPROPERTY()
		TArray<FVector> EnemyLocations;
	UPROPERTY()
		TArray<FRotator> EnemyRotations;
	UPROPERTY()
		TArray<bool> EnemyRagdollStates;
	UPROPERTY()
		TArray<FVector> EnemyMeshLocations;
	UPROPERTY()
		TArray<FVector> EnemyMeshVelocities;
	
};
