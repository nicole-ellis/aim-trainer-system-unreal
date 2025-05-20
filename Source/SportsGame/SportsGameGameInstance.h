// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SportsGameGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPORTSGAME_API USportsGameGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	bool bLoadSave = false;
};
