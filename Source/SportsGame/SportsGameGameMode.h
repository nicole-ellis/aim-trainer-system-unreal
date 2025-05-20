// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SportsGameSaveGame.h"
#include "GameFramework/GameModeBase.h"
#include "SportsGameGameMode.generated.h"

UCLASS(minimalapi)
class ASportsGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASportsGameGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> EnemyClass;

	void Save(USportsGameSaveGame* SaveGame);
	void Load(USportsGameSaveGame* LoadedGame);
	UFUNCTION()
		void PostBeginPlay();
	
};



