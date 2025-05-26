// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "Engine/World.h"

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
	bCanSpawn = true;
	ShotsFired = 0;
	ShotsHit = 0;
	ActiveTargets.Empty();

	SpawnTargets();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UTargetSpawner::EndAimMode, GameDuration, false);
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
	UE_LOG(LogTemp, Warning, TEXT("SpawnTargets() called"));
	
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
		
		float X = Origin.X + (FMath::FRand() * HorizontalSpread);
		float Y = Origin.Y + FMath::RandRange(-300.0f, 800.0f);
		float Z = Origin.Z + FMath::RandRange(VerticalMin, VerticalMax);

		FVector SamplePoint(X, Y, Z);
		FNavLocation NavLocation;

		UE_LOG(LogTemp, Warning, TEXT("Trying to spawn at: X=%.1f Y=%.1f Z=%.1f"), X, Y, Z);

		if (NavSys && NavSys->ProjectPointToNavigation(SamplePoint, NavLocation))
		{
			AActor* Spawned = GetWorld()->SpawnActor<AActor>(TargetToSpawn, NavLocation.Location, FRotator::ZeroRotator);
			if (Spawned)
			{
				UE_LOG(LogTemp, Error, TEXT("Spawned: %s"), *Spawned->GetName());
				
				Spawned->Tags.Add("Target");
				Spawned->OnDestroyed.AddDynamic(this, &UTargetSpawner::OnTargetDestroyed);
				ActiveTargets.Add(Spawned);
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

