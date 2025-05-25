// Fill out your copyright notice in the Description page of Project Settings.


#include "AimTrainingCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interactable.h"
#include "Android/AndroidSystemIncludes.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAimTrainingCharacter::AAimTrainingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	// Create FPS Camera
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetMesh()); // Attaches to skeletal mesh
	FPSCamera->bUsePawnControlRotation = true;

	// Allow controller input to affect character rotation
	bUseControllerRotationYaw = false;
}

void AAimTrainingCharacter::EnterAimMode()
{
	// Disable movement
	GetCharacterMovement()->DisableMovement();
	bIsZoomed = true;
	
	// Reset counters
	ShotsFired = 0;
	ShotsHit = 0;
	Attempts++;

	// Switch camera logic to FPS camera
	if (FollowCamera)
	{
		FollowCamera->Deactivate();
	}

	if (FPSCamera)
	{
		FPSCamera->Activate(true);
	}
	
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->SetViewTargetWithBlend(this, 1.0f); // Ensures player camera is set to this actor

		PC->SetViewTarget(this);
	}
}

void AAimTrainingCharacter::Fire()
{
	// Only allow firing if in aim mode
	if (!bIsZoomed || bIsInCrashOutMode)
		return;

	ShotsFired++;

	// Get player view
	FVector Start;
	FRotator Rotation;
	GetActorEyesViewPoint(Start, Rotation);

	FVector End = Start + (Rotation.Vector() * 5000.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // So player doesnt hit themselves

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		ShotsHit++;
	}

	CalculateAccuracy();
}	

void AAimTrainingCharacter::CalculateAccuracy()
{
}

void AAimTrainingCharacter::ResetSession()
{
}

float AAimTrainingCharacter::GetAccuracy() const
{
	return 0.0f; // Temp
}

void AAimTrainingCharacter::Use()
{
	// Line trace to interact with objects
	FVector Start;
	FRotator Rotation;
	GetActorEyesViewPoint(Start, Rotation);

	FVector End = Start + (Rotation.Vector() * 500.0f);

	// Debug line
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
		
		if (HitActor && HitActor->Implements<UInteractable>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor implements Interactable!"));
			IInteractable::Execute_Interact(HitActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor does not implement Interactable!"));
		}
	}
}

// Called when the game starts or when spawned
void AAimTrainingCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem && AimTrainerMappingContext)
		{
			Subsystem->ClearAllMappings(); // Clear IMC_Default
			Subsystem->AddMappingContext(AimTrainerMappingContext, 0);
		}
	}

	// Default camera setup
	if (FPSCamera)
	{
		FPSCamera->Deactivate();
	}

	if (FollowCamera)
	{
		FollowCamera->Activate();
	}
	
}

void AAimTrainingCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller && (!bIsZoomed || !bIsInCrashOutMode))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAimTrainingCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Look Axis Vector: %s"), *LookAxisVector.ToString());

	if (Controller)
	{
		AddControllerYawInput(-LookAxisVector.X * Sensitivity);
		AddControllerPitchInput(LookAxisVector.Y * Sensitivity);
	}
}


// Called every frame
void AAimTrainingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAimTrainingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAimTrainingCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAimTrainingCharacter::Look);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AAimTrainingCharacter::Fire);

		// Using
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &AAimTrainingCharacter::Use);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

