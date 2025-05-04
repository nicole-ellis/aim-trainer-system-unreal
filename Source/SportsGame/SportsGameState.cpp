// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsGameState.h"
#include "InGameUI.h"
#include "SportsGameCharacter.h"

void ASportsGameState::Score(bool bPlayerTeam, int Amount)
{
	if (bPlayerTeam)
	{
		PlayerScore += Amount;
	}
	else
	{
		EnemyScore += Amount;
	}
	if (Cast<ASportsGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->InGameUI)
	{
		Cast<ASportsGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->InGameUI->UpdateValues();
	}
}
