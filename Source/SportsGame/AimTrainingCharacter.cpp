// Fill out your copyright notice in the Description page of Project Settings.


#include "AimTrainingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAimTrainingCharacter::AAimTrainingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called when the game starts or when spawned
void AAimTrainingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAimTrainingCharacter::Fire);
}

