// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SportsGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API ASportsGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	int PlayerScore = 0;
	int EnemyScore = 0;

		void Score(bool bPlayerTeam, int Amount);
};
