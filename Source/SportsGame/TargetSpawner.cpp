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

// Called when the game starts
void UTargetSpawner::BeginPlay()
{
	
	Super::BeginPlay();
	
}


// Called every frame
void UTargetSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UTargetSpawner::SpawnTargets()
{
	if (!TargetToSpawn)
		return;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
		return;

	AActor* Owner = GetOwner();
	if (!Owner)
		return;
	
	FVector Origin = GetOwner()->GetActorLocation();

	for (int i = 0; i < TargetCount; ++i)
	{
		// Spread along X-axis
		float X = Origin.X + (i * HorizontalSpread / TargetCount);

		// Random Y and Z for horizontal/vertical varience
		float Y = Origin.Y + FMath::RandRange(-300.0f, 300.0f);
		float Z = Origin.Z + FMath::RandRange(VerticalMin, VerticalMax);

		FVector SamplePoint(X, Y, Z);

		FNavLocation NavLocation;
		if (NavSys->ProjectPointToNavigation(SamplePoint, NavLocation))
		{
			GetWorld()->SpawnActor<AActor>(TargetToSpawn, NavLocation.Location, FRotator::ZeroRotator);
		}
	}
}

