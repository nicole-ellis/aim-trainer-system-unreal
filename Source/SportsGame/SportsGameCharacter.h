// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HitTextUI.h"
#include "SportsGameCharacter.generated.h"

class UInGameUI;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASportsGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ASportsGameCharacter();

	UPROPERTY(EditAnywhere)
		float KickOffset = 100;
	UPROPERTY(EditAnywhere)
		float KickPower = 5000;
	UPROPERTY(EditAnywhere)
		float SprintAmount = 500;

	UPROPERTY(EditAnywhere)
		UInputAction* KickAction;
	UPROPERTY(EditAnywhere)
		UInputAction* SprintAction;

	UFUNCTION()
		void Kick();
	UFUNCTION()
		void SprintStart();
	UFUNCTION()
		void SprintEnd();

	UPROPERTY(EditAnywhere)
		UInputAction* UseAction;
	UPROPERTY(EditAnywhere)
		float UseDistance = 1000;
	UFUNCTION()
		void Use();

	UPROPERTY(EditAnywhere)
	float MaxStamina = 100;
	UPROPERTY(EditAnywhere)
	float StaminaRate = 20;
	UPROPERTY(EditAnywhere)
	float StaminaRefreshCooldown = 2;
	UPROPERTY(EditAnywhere)
	float KickCooldown = 0.5;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameUI> InGameUIClass;
	UPROPERTY()
	UInGameUI* InGameUI;
	float CurrentStamina = MaxStamina;
	float CurrentKickTimer = KickCooldown;
	bool bIsSprinting = false;
	bool bIsStaminaRegen = false;
	FTimerHandle StaminaRegenHandle;

	UFUNCTION()
	void StartStaminaRegen();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void ReduceStamina(float Amount);

	UPROPERTY(EditAnywhere)
		TSubclassOf<UHitTextUI> HitTextClass;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

