// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AimTrainingCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SPORTSGAME_API AAimTrainingCharacter : public ACharacter
{
	GENERATED_BODY()

	// Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AimTrainerMappingContext;

	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Fire Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	// Cameras
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPSCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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

	// Called for movement input
	void Move(const FInputActionValue& Value);
	
	// Called for looking input
	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
