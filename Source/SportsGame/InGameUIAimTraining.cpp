// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUIAimTraining.h"

void UInGameUIAimTraining::UpdateValues()
{
	if (Player)
	{
		AccuracyText->SetText(FText::FromString(FString::FromInt(Player->GetAccuracy())));
		ShotsHitText->SetText(FText::FromString(FString::FromInt(Player->ShotsHit)));
		AttemptsText->SetText(FText::FromString(FString::FromInt(Player->Attempts)));
	}
}
