// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ball.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UCLASS()
class SPORTSGAME_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* GoalMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Collider;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABall> BallClass;
	UPROPERTY(EditAnywhere)
		FVector BallSpawn;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int BodyIndex, bool bSweep, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
