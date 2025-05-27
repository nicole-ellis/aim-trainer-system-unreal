// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimTrainingCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InGameUIAimTraining.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API UInGameUIAimTraining : public UUserWidget
{
	GENERATED_BODY()

public:

	 // Values to be displayed to the player
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ShotsHitText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AttemptsText;
	
	UPROPERTY()
		AAimTrainingCharacter* Player;

	void UpdateValues();
};
