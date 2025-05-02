// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Called every frame
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuClass;

	virtual void BeginPlay() override;
};
