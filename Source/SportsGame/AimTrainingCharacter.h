// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AimTrainingCharacter.generated.h"

UCLASS()
class SPORTSGAME_API AAimTrainingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAimTrainingCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sensitivity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ShotsFired;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ShotsHit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attempts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsZoomed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInCrashOutMode;

	// Called when player presses start button
	void EnterAimMode();

	// Called when player fires
	void Fire();

	// Called after each shot
	void CalculateAccuracy();

	// Called to reset attempt
	void ResetSession();

	// Called from logic to get accuracy %
	UFUNCTION(BlueprintCallable)
	float GetAccuracy() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
