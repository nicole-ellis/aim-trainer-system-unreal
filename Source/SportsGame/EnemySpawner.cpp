// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Bounds"));
	RootComponent = SpawnBounds;
}

void AEnemySpawner::SpawnEnemy()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, Enemies);
	if (EnemyClass && Enemies.Num() < MaxEnemies)
	{
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBounds->GetComponentLocation(), SpawnBounds->GetScaledBoxExtent());
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		GetWorld()->SpawnActor(EnemyClass, &SpawnLocation, &SpawnRotation, SpawnParameters);
	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnFrequency, true);
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

