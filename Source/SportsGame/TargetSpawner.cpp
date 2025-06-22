// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSpawner.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTargetSpawner::UTargetSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTargetSpawner::EndAimMode()
{
	bCanSpawn = false;
	UE_LOG(LogTemp, Warning, TEXT("Aim mode ended."));
}

void UTargetSpawner::RegisterShot(bool bHit)
{
	ShotsFired++;

	if (bHit)
	{
		ShotsHit++;
	}

	// Live stats
	float Accuracy = (ShotsFired > 0) ? (float)ShotsHit / ShotsFired : 0.0f;
	UE_LOG(LogTemp, Log, TEXT("Shots: %d | Hits: %d | Accuracy: %.1f%%"), ShotsFired, ShotsHit, Accuracy * 100.0f);
}

void UTargetSpawner::OnTargetDestroyed(AActor* DestroyedTarget)
{
	if (!bCanSpawn)
		return;

	ActiveTargets.Remove(DestroyedTarget); // Remove from tracking
	SpawnTargets(); // Maintain target count
}

void UTargetSpawner::BeginTraining()
{
	// Enters player into aim training and resets all values
	bCanSpawn = true;
	ShotsFired = 0;
	ShotsHit = 0;
	ActiveTargets.Empty();

	switch (LevelMode)
	{
	case EAimTrainerLevelMode::DropTargets:SpawnFallingTargets();
		break;

	case EAimTrainerLevelMode::ExplodingTargets:SpawnShootingTargets();
		break;

	case EAimTrainerLevelMode::AIMovingTargets:SpawnAITargets();
		break;
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UTargetSpawner::EndAimMode, GameDuration, false);
	}
}

void UTargetSpawner::SpawnFallingTargets()
{
	// So that the targets don't keep spawning and crashing the game
	int MaxAttempts = 50;
	int Attempts = 0;
	
	if (!TargetToSpawn || !bCanSpawn)
		return;

	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	FVector Origin = Owner->GetActorLocation();

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
		return;

	while (ActiveTargets.Num() < TargetCount && Attempts < MaxAttempts)
	{
		Attempts++;
		
		// Spawn targets within a certain range
		float X = Origin.X + 800.0f; // Depth
		float Y = Origin.Y + FMath::RandRange(-HorizontalSpread / 2, HorizontalSpread / 2); // Left/right spread
		float Z = Origin.Z + FMath::RandRange(VerticalMin, VerticalMax); // Vertical Range

		bool bTooClose = false;
		for (const AActor* Existing : ActiveTargets)
		{
			if (FVector::Dist(Existing->GetActorLocation(), FVector(X,Y,Z)) < 120.0f)
			{
				bTooClose = true;
				break;
			}
		}
		
		FVector SamplePoint(X, Y, Z);
		FNavLocation NavLocation;

		UE_LOG(LogTemp, Warning, TEXT("Trying to spawn at: X=%.1f Y=%.1f Z=%.1f"), X, Y, Z);

		if (NavSys && NavSys->ProjectPointToNavigation(SamplePoint, NavLocation))
		{
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(TargetToSpawn, NavLocation.Location, FRotator::ZeroRotator);
			if (Spawned)
			{
				UE_LOG(LogTemp, Error, TEXT("Spawned: %s"), *Spawned->GetName());

				Spawned->Tags.Add("Target"); // Targets spawn with the tag "Target" so that the player can only break targets
				Spawned->OnDestroyed.AddDynamic(this, &UTargetSpawner::OnTargetDestroyed);
				ActiveTargets.Add(Spawned);

				// Apply launch force to simulate it shooting out
				UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Spawned->GetRootComponent());
				if (RootComp && RootComp->IsSimulatingPhysics())
				{
					FVector LaunchDirection = FVector(0.f, FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(0.5f, 1.5f)).GetSafeNormal();
					RootComp->AddImpulse(LaunchDirection * LaunchStrength, NAME_None, true);
				}
				
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Spawn failed - TargetToSpawn was nullptr or not valid."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to project to navmesh."));
		}
	}
}

void UTargetSpawner::SpawnShootingTargets()
{
	if (!TargetToSpawn || !bCanSpawn)
		return;

	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	FVector Origin = Owner->GetActorLocation();
	const float Radius = 200.0f;

	for (int i = 0; i < TargetCount; ++i)
	{
		FVector SpawnLocation = Origin;

		AActor* Spawned = GetWorld()->SpawnActor<AActor>(TargetToSpawn, SpawnLocation, FRotator::ZeroRotator);
		if (Spawned)
		{
			Spawned->Tags.Add("Target");
			Spawned->OnDestroyed.AddDynamic(this, &UTargetSpawner::OnTargetDestroyed);
			ActiveTargets.Add(Spawned);

			UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Spawned->GetRootComponent());
			if (RootComp && RootComp->IsSimulatingPhysics())
			{
				// Launch outward in random direction
				FVector RandomDir = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(0.3f, 1.2f)).GetSafeNormal();
				RootComp->AddImpulse(RandomDir * LaunchStrength * 2.5f, NAME_None, true);
			}
		}
	}
}

void UTargetSpawner::SpawnAITargets()
{
	if (!TargetToSpawn || !bCanSpawn)
		return;

	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	FVector Origin = Owner->GetActorLocation();

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
		return;

	for (int i = 0; i < TargetCount; i++)
	{
		FNavLocation NavLocation;
		bool bFound = NavSys->GetRandomPointInNavigableRadius(Origin, 1000.0f, NavLocation);

		if (bFound)
		{
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(TargetToSpawn, NavLocation.Location, FRotator::ZeroRotator);
			if (Spawned)
			{
				Spawned->Tags.Add("Target");
				Spawned->OnDestroyed.AddDynamic(this, &UTargetSpawner::OnTargetDestroyed);
				ActiveTargets.Add(Spawned);

				// Move to another random nav point
				AAIController* AI = Cast<AAIController>(Spawned->GetInstigatorController());
				if (!AI) AI = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());

				if (AI)
				{
					FNavLocation MoveTarget;
					if (NavSys->GetRandomPointInNavigableRadius(NavLocation.Location, 500.0f, MoveTarget))
					{
						AI->Possess(Cast<APawn>(Spawned));
						AI->MoveToLocation(MoveTarget.Location);
					}
				}
			}
		}
	}
}

// Called when the game starts
void UTargetSpawner::BeginPlay()
{
	Super::BeginPlay();

	// GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UTargetSpawner::EndAimMode, GameDuration, false);
	// bCanSpawn = true;

	// SpawnTargets();
}


// Called every frame
void UTargetSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UTargetSpawner::SpawnTargets()
{
	
	
}

void UTargetSpawner::DestroyAllTargets()
{
	// Just in case Destroy() immediately triggers
	TArray<AActor*> ToDestroy;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetToSpawn, ToDestroy);

	for (AActor* Target : ToDestroy)
	{
		if (IsValid(Target)) // Checks not null and not pending 
		{
			Target->OnDestroyed.RemoveAll(this);
			Target->Destroy();
		}
	}

	// Clear tracking array
	ActiveTargets.Empty();
}

