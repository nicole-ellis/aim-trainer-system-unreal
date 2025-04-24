// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Ball)
	{
		FVector TowardsBall = Ball->GetActorLocation() - GetActorLocation();
		AddMovementInput(TowardsBall);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

