// Fill out your copyright notice in the Description page of Project Settings.


#include "StartButton.h"
#include "AimTrainingCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStartButton::AStartButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Interaction Box
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	SetRootComponent(InteractionBox);
	InteractionBox->SetBoxExtent(FVector(50.0f, 50.0f, 20.0f)); 
	InteractionBox->SetCollisionProfileName(TEXT("Trigger"));
	
	// Mesh
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);

	bPlayerInRange = false;
}

// Called when the game starts or when spawned
void AStartButton::BeginPlay()
{
	Super::BeginPlay();

	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AStartButton::AStartButton::OnOverlapBegin);
}

// Called when player enters interaction box
void AStartButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AAimTrainingCharacter::StaticClass()))
	{
		bPlayerInRange = true;
	}
}

void AStartButton::Interact_Implementation(AActor* Interactor)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (AAimTrainingCharacter* AimChar = Cast<AAimTrainingCharacter>(Player))
	{
		AimChar->EnterAimMode();
	}
}

// Called every frame
void AStartButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

