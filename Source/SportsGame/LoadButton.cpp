// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadButton.h"
#include "SportsGameGameInstance.h"
#include "Kismet/GameplayStatics.h"


ULoadButton::ULoadButton()
{
	OnClicked.AddDynamic(this, &ULoadButton::OnButtonClick);
}

void ULoadButton::OnButtonClick()
{
	if (UGameplayStatics::DoesSaveGameExist("TestSave", 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), MapToOpen);
		USportsGameGameInstance* GameInstance = Cast<USportsGameGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->bLoadSave = true;
		}
	}
}
