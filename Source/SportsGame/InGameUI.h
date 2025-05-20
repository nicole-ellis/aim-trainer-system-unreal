// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SportsGameState.h"
#include "InGameUI.generated.h"

class ASportsGameCharacter;

UCLASS()
class SPORTSGAME_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()

public:	

	UPROPERTY(meta = (BindWidget))
		UProgressBar* StaminaBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* KickCooldownBar;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentStaminaText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MaxStaminaText;

	UPROPERTY()
		ASportsGameCharacter* Player;

	void UpdateValues();

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerScoreText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* EnemyScoreText;
	UPROPERTY()
		ASportsGameState* GameState;

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* EXPBar;
};
