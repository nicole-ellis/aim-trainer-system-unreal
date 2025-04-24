// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemySpawner.generated.h"

UCLASS()
class SPORTSGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere)
		UBoxComponent* SpawnBounds;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> EnemyClass;
	UPROPERTY(EditAnywhere)
		float SpawnFrequency = 3;
	UPROPERTY(EditAnywhere)
		int MaxEnemies = 5;
	FTimerHandle SpawnTimerHandle;

	UFUNCTION()
		void SpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
