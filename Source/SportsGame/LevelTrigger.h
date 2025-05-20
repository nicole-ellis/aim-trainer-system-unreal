// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LevelTrigger.generated.h"

UCLASS()
class SPORTSGAME_API ALevelTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTrigger();
	
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerVolume;
	UPROPERTY(EditAnywhere)
		FName LevelToLoad;
	UPROPERTY(EditAnywhere)
		bool bLoadLevel;

	UFUNCTION()
		void StartLevelStreaming(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bSweep, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
