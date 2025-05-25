// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "TargetSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPORTSGAME_API UTargetSpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
		TSubclassOf<AActor> TargetToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		int TargetCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		float HorizontalSpread = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		float VerticalMin = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
		float VerticalMax = 500.0f;
	
	UFUNCTION(BlueprintCallable)
		void SpawnTargets();
};
