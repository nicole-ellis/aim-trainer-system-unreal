// Copyright Epic Games, Inc. All Rights Reserved.

#include "Interactable.h"
#include "SportsGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Ball.h"
#include "Enemy.h"
#include "InGameUI.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASportsGameCharacter

ASportsGameCharacter::ASportsGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASportsGameCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASportsGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASportsGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASportsGameCharacter::Look);

		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Started, this, &ASportsGameCharacter::Kick);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASportsGameCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASportsGameCharacter::SprintEnd);

		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &ASportsGameCharacter::Use);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASportsGameCharacter::Kick()
{
	if (CurrentKickTimer >= KickCooldown)
	{
		CurrentKickTimer = 0;
		
		FVector Location = GetActorLocation() + GetActorForwardVector() * KickOffset;
		TArray<FHitResult> Hits;
		FCollisionShape Cube = FCollisionShape::MakeBox(FVector(KickOffset));
		GetWorld()->SweepMultiByChannel(Hits, Location, Location, GetActorQuat(), ECC_WorldDynamic, Cube);
		DrawDebugBox(GetWorld(), Location, Cube.GetExtent(), FColor::Blue);

		for (FHitResult Hit : Hits)
		{
			if (Hit.GetActor() != this)
			{
				ABall* Ball = Cast<ABall>(Hit.GetActor());
				if (Ball)
				{
					FVector LaunchDirection = Ball->BallMesh->GetComponentLocation() - GetActorLocation();
					LaunchDirection.Normalize();
					LaunchDirection *= 3;
					LaunchDirection += FVector::UpVector;
					Ball->BallMesh->AddImpulse(LaunchDirection*KickPower);
				}
				AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());
				if (Enemy)
				{
					Enemy->Ragdoll();
					FVector LaunchDirection = Enemy->GetActorLocation() - GetActorLocation();
					LaunchDirection.Normalize();
					LaunchDirection *= 3;
					LaunchDirection += FVector::UpVector;
					Enemy->GetMesh()->AddImpulse(LaunchDirection*KickPower);
				}
			}
		}
	}
}

void ASportsGameCharacter::SprintStart()
{
	if (!bIsSprinting)
	{
		if (CurrentStamina > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed += SprintAmount;
			bIsSprinting = true;
			bIsStaminaRegen = false;
		}
	}	
}

void ASportsGameCharacter::SprintEnd()
{
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed -= SprintAmount;
		bIsSprinting = false;
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, this, &ASportsGameCharacter::StartStaminaRegen, StaminaRefreshCooldown);
	}
}

void ASportsGameCharacter::Use()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetFollowCamera()->GetForwardVector() * UseDistance;

	FHitResult HitData;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	TraceParams.TraceTag = FName("Use Trace Tag");
	GetWorld()->DebugDrawTraceTag = TraceParams.TraceTag;

	bool bSweep = GetWorld()->LineTraceSingleByChannel(HitData, Start, End, ECC_Visibility, TraceParams);

	if (bSweep)
	{
		if (HitData.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("We hit: %s"), *HitData.GetActor()->GetName());
			if (HitData.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				IInteractable::Execute_Interact(HitData.GetActor());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor not interactable!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("What happened?!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't hit an actor!"));
	}
}

void ASportsGameCharacter::StartStaminaRegen()
{
	if (!bIsSprinting)
	{
		bIsStaminaRegen = true;
	}
}

void ASportsGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSprinting)
	{
		ReduceStamina(StaminaRate * DeltaSeconds);
	}

	if (bIsStaminaRegen)
	{
		ReduceStamina(-StaminaRate * DeltaSeconds * 2);
	}

	if (CurrentKickTimer < KickCooldown)
	{
		CurrentKickTimer += DeltaSeconds;
		if (InGameUI)
		{
			InGameUI->UpdateValues();
		}
	}
}

void ASportsGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InGameUIClass)
	{
		InGameUI = Cast<UInGameUI>(CreateWidget(GetGameInstance(), InGameUIClass));
		InGameUI->Player = this;
		InGameUI->UpdateValues();
		InGameUI->AddToViewport();
	}
}

void ASportsGameCharacter::ReduceStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0, MaxStamina);
	if (CurrentStamina == 0)
	{
		SprintEnd();
	}

	if (CurrentStamina == MaxStamina)
	{
		bIsStaminaRegen = false;
	}

	if (InGameUI)
	{
		InGameUI->UpdateValues();
	}
}

void ASportsGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASportsGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
