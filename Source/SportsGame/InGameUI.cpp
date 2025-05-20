// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "SportsGameCharacter.h"

void UInGameUI::UpdateValues()
{
	if (Player)
	{
		StaminaBar->SetPercent(Player->CurrentStamina / Player->MaxStamina);
		CurrentStaminaText->SetText(FText::FromString(FString::FromInt(Player->CurrentStamina)));
		MaxStaminaText->SetText(FText::FromString(FString::FromInt(Player->MaxStamina)));
		KickCooldownBar->SetPercent(Player->CurrentKickTimer / Player->KickCooldown);

		EXPBar->SetPercent((float)Player->CurrentEXP / Player->EXPToLevel);

		if (GameState)
		{
			PlayerScoreText->SetText(FText::AsNumber(GameState->PlayerScore));
			EnemyScoreText->SetText(FText::AsNumber(GameState->EnemyScore));
		}
	}
}

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = Cast<ASportsGameState>(GetWorld()->GetGameState());
}
