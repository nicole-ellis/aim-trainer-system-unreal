// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBTController.h"
#include "Ball.h"
#include "SportsGameState.h"
#include "NavigationPath.h"
#include "SportsGameCharacter.h"

AEnemyBTController::AEnemyBTController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Config")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
}

void AEnemyBTController::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyBTController::OnSensesUpdated);
}

void AEnemyBTController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}

	if (TargetBall)
	{
		BlackboardComponent->SetValueAsVector("BallPosition", TargetBall->GetActorLocation());
	}
}

FRotator AEnemyBTController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	
	return Super::GetControlRotation();
}

void AEnemyBTController::GenerateNewRandomLocation()
{
	if (NavigationSystem && GetPawn())
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), PatrolDistance, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AEnemyBTController::OnSensesUpdated(const TArray<AActor*>& UpdatedActors)
{
	TargetBall = nullptr;
	BlackboardComponent->SetValueAsBool("ChaseBall", false);

	TargetPlayer = nullptr;
	BlackboardComponent->SetValueAsBool("ChasePlayer", false);

	for (AActor* Actor : UpdatedActors)
	{
		if (APawn* SensedPawn = Cast<APawn>(Actor))
		{
			if (SensedPawn->IsPlayerControlled())
			{
				TargetPlayer = SensedPawn;
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
			}
		}
		if (ABall* SensedBall = Cast<ABall>(Actor))
		{
			TargetBall = SensedBall;
			BlackboardComponent->SetValueAsBool("ChaseBall", true);
			BlackboardComponent->SetValueAsVector("BallPosition", TargetBall->GetActorLocation());
		}
	}
}


void AEnemyBTController::UpdateStunCheck()
{
	ASportsGameState* GameState = Cast<ASportsGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		if (GameState->EnemyScore < GameState->PlayerScore)
		{
			if (TargetPlayer && GetPawn())
			{
				if (FVector::Dist(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation()) < 200)
				{
					BlackboardComponent->SetValueAsBool("StunPossible", true);
				}
			}
		}
		else
		{
			BlackboardComponent->SetValueAsBool("StunPossible", false);
		}
	}
}

void AEnemyBTController::StunPlayer()
{
	if (TargetPlayer)
	{
		if (Cast<ASportsGameCharacter>(TargetPlayer))
		{
			Cast<ASportsGameCharacter>(TargetPlayer)->StartStun();
		}
	}
}