// Fill out your copyright notice in the Description page of Project Settings.

#include "NavLinkCustomComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "JumpPoint.h"

void AJumpPoint::BeginPlay()
{
	Super::BeginPlay();

	CopyLinkPoints();
	OnSmartLinkReached.AddDynamic(this, &AJumpPoint::LinkReached);
}

void AJumpPoint::LinkReached(AActor* Agent, const FVector& Destination)
{
	ACharacter* Enemy = Cast<ACharacter>(Agent);
	if (Enemy)
	{
		FVector LaunchVelocity;
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), LaunchVelocity, Enemy->GetMesh()->GetComponentLocation(), Destination);
		LaunchVelocity.Z *= 1.2;
		Enemy->LaunchCharacter(LaunchVelocity, true, true);
	}
}

void AJumpPoint::CopyLinkPoints()
{
	if (PointLinks.Num() && GetSmartLinkComp())
	{
		GetSmartLinkComp()->Modify();
		GetSmartLinkComp()->SetLinkData(PointLinks[0].Left, PointLinks[0].Right, PointLinks[0].Direction);
	}
}
