// Copyright Epic Games, Inc. All Rights Reserved.

#include "SportsGameGameMode.h"

#include "InGameUI.h"
#include "SportsGameCharacter.h"
#include "SportsGameGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ASportsGameGameMode::ASportsGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASportsGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ASportsGameGameMode::PostBeginPlay);
}

void ASportsGameGameMode::Save(USportsGameSaveGame* SaveGame)
{
	ASportsGameCharacter* PlayerCharacter = Cast<ASportsGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerCharacter && SaveGame)
	{
		SaveGame->PlayerPosition = PlayerCharacter->GetActorLocation();
		SaveGame->PlayerRotation = PlayerCharacter->GetActorRotation();
		SaveGame->PlayerCurrentStamina = PlayerCharacter->CurrentStamina;
		SaveGame->PlayerMaxStamina = PlayerCharacter->MaxStamina;
		SaveGame->PlayerKickPower = PlayerCharacter->KickPower;
		SaveGame->PlayerCurrentEXP = PlayerCharacter->CurrentEXP;
		SaveGame->PlayerEXPToLevel = PlayerCharacter->EXPToLevel;

		TArray<AActor*> Enemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
		for (int i = 0; i < Enemies.Num(); i++)
		{
			AEnemy* CurrentEnemy = Cast<AEnemy>(Enemies[i]);
			SaveGame->EnemyLocations.Add(CurrentEnemy->GetActorLocation());
			SaveGame->EnemyRotations.Add(CurrentEnemy->GetActorRotation());
			SaveGame->EnemyRagdollStates.Add(CurrentEnemy->GetMesh()->GetCollisionProfileName() == "Ragdoll");
			SaveGame->EnemyMeshLocations.Add(CurrentEnemy->GetMesh()->GetComponentLocation());
			SaveGame->EnemyMeshVelocities.Add(CurrentEnemy->GetMesh()->GetComponentVelocity());
		}
	}
}

void ASportsGameGameMode::Load(USportsGameSaveGame* LoadedGame)
{
	ASportsGameCharacter* PlayerCharacter = Cast<ASportsGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerCharacter && LoadedGame)
	{
		PlayerCharacter->SetActorLocationAndRotation(LoadedGame->PlayerPosition, LoadedGame->PlayerRotation);
		PlayerCharacter->KickPower = LoadedGame->PlayerKickPower;
		PlayerCharacter->CurrentStamina = LoadedGame->PlayerCurrentStamina;
		PlayerCharacter->MaxStamina = LoadedGame->PlayerMaxStamina;
		PlayerCharacter->CurrentEXP = LoadedGame->PlayerCurrentEXP;
		PlayerCharacter->EXPToLevel = LoadedGame->PlayerEXPToLevel;
		PlayerCharacter->InGameUI->UpdateValues();

		for (int i = 0; i < LoadedGame->EnemyLocations.Num(); i++)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AEnemy* SpawnedEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(EnemyClass, nullptr, nullptr, SpawnParameters));
			if (SpawnedEnemy)
			{
				SpawnedEnemy->SetActorLocationAndRotation(LoadedGame->EnemyLocations[i], LoadedGame->EnemyRotations[i]);
				if (LoadedGame->EnemyRagdollStates[i])
				{
					SpawnedEnemy->Ragdoll();
					SpawnedEnemy->GetCharacterMovement()->GravityScale = 0;
					SpawnedEnemy->GetMesh()->SetWorldLocation(LoadedGame->EnemyMeshLocations[i], false, nullptr, ETeleportType::TeleportPhysics);
					SpawnedEnemy->GetMesh()->SetAllPhysicsLinearVelocity(LoadedGame->EnemyMeshVelocities[i], true);
				}
			}
		}
	}
}

void ASportsGameGameMode::PostBeginPlay()
{
	USportsGameGameInstance* GameInstance = Cast<USportsGameGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (GameInstance->bLoadSave)
		{
			USportsGameSaveGame* LoadedGame = Cast<USportsGameSaveGame>(UGameplayStatics::LoadGameFromSlot("TestSave",0));
			if (LoadedGame)
			{
				Load(LoadedGame);
			}
		}
	}
}
