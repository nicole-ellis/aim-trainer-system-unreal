// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = GoalMesh;
	Collider->SetupAttachment(GoalMesh);
}

void AGoal::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int BodyIndex, bool bSweep, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		ABall* Ball = Cast<ABall>(OtherActor);
		if (Ball)
		{
			FVector BallDirection = Ball->GetVelocity().GetSafeNormal();
			FVector GoalDirection = Collider->GetForwardVector().GetSafeNormal();
			if (BallDirection.Dot(GoalDirection) < 0)
			{
				// Destroy ball then spawn new one in centre
				Ball->Destroy();
				GetWorld()->SpawnActor(BallClass, &BallSpawn);
			}
		}
	}
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnBeginOverlap);
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

