// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BrainComponent.h"
#include "EnemyBTController.h"
#include "EnemySpawner.h"
#include "SportsGameCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AEnemy::Ragdoll()
{
	if (Cast<AEnemyBTController>(GetController()))
	{
		Cast<AEnemyBTController>(GetController())->BrainComponent->PauseLogic("Ragdolling!");
	}
	
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AEnemy::StopRagdoll, RagdollTime);
}

void AEnemy::StopRagdoll()
{
	if (SpawnerBounds)
	{
		if (!UKismetMathLibrary::IsPointInBox(GetMesh()->GetComponentLocation(), SpawnerBounds->GetComponentLocation(), SpawnerBounds->GetScaledBoxExtent()))
		{
			Destroy();
			return;
		}
	}
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("Character Name");
	GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("pelvis"));
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88), FRotator(0,-90,0));
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	//Increase this is enemy sticking in the ground on un-ragdoll
	AddActorLocalOffset(FVector(0,0,50));
	
	if (Cast<AEnemyBTController>(GetController()))
	{
		Cast<AEnemyBTController>(GetController())->BrainComponent->ResumeLogic("Ragdolling!");
	}

	GetCharacterMovement()->GravityScale = 1;
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Player)
	{
		Player->AddEXP(EXPAmount);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AActor* Spawner = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass());
	if (Spawner)
	{
		SpawnerBounds = Cast<AEnemySpawner>(Spawner)->SpawnBounds;
	}

	Player = Cast<ASportsGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

