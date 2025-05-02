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
	}
}
