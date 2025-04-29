// Fill out your copyright notice in the Description page of Project Settings.


#include "Trapdoor.h"

// Sets default values
ATrapdoor::ATrapdoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	LeftHinge = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Left Hinge"));
	RightHinge = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Right Hinge"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LeftDoor->SetupAttachment(RootComponent);
	RightDoor->SetupAttachment(RootComponent);
	LeftHinge->SetupAttachment(RootComponent);
	RightHinge->SetupAttachment(RootComponent);
}

void ATrapdoor::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	LeftDoor->SetSimulatePhysics(true);
	LeftDoor->AddImpulse(FVector::DownVector);
	RightDoor->SetSimulatePhysics(true);
	RightDoor->AddImpulse(FVector::DownVector);
	GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &ATrapdoor::ResetDoor, ResetTime);
}

void ATrapdoor::ResetDoor()
{
	LeftDoor->SetSimulatePhysics(false);
	LeftDoor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	LeftDoor->SetWorldTransform(LeftInitial);
	RightDoor->SetSimulatePhysics(false);
	RightDoor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	RightDoor->SetWorldTransform(RightInitial);
}

// Called when the game starts or when spawned
void ATrapdoor::BeginPlay()
{
	Super::BeginPlay();

	LeftInitial = LeftDoor->GetComponentTransform();
	LeftDoor->SetSimulatePhysics(false);
	LeftDoor->SetCollisionObjectType(ECC_GameTraceChannel1);
	LeftDoor->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	LeftDoor->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

	RightInitial = RightDoor->GetComponentTransform();
	RightDoor->SetSimulatePhysics(false);
	RightDoor->SetCollisionObjectType(ECC_GameTraceChannel1);
	RightDoor->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	RightDoor->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
}

// Called every frame
void ATrapdoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

