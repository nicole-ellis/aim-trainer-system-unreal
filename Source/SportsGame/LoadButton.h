// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "LoadButton.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API ULoadButton : public UButton
{
	GENERATED_BODY()

public:

	ULoadButton();
	UFUNCTION()
		void OnButtonClick();
	UPROPERTY(EditAnywhere)
		FName MapToOpen;
	
};
