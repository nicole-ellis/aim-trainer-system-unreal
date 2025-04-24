// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	RootComponent = BallMesh;

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	BallMesh->SetSimulatePhysics(true);

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
	for (int i = 0; i < Enemies.Num(); i++)
	{
		Cast<AEnemy>(Enemies[i])->Ball = this;
	}
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

