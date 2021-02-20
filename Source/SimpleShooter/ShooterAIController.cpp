// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		//auto Blackboard = GetBlackboardComponent();
		//Blackboard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		Blackboard->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// NOTE(alison) first iteration, without Blackboards
	//if (LineOfSightTo(PlayerPawn))
	//{
	//	SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	//	MoveToActor(PlayerPawn, AcceptanceRadius);
	//}
	//else
	//{
	//	ClearFocus(EAIFocusPriority::Gameplay);
	//	StopMovement();
	//}


	// NOTE(alison) second iteration, without BTServices
	// this is implemented by BTService_PlayerLocationIfSeen
	//if (LineOfSightTo(PlayerPawn))
	//{
	//	auto PlayerLocation = PlayerPawn->GetActorLocation();
	//	Blackboard->SetValueAsVector(TEXT("PlayerLocation"), PlayerLocation);
	//	Blackboard->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerLocation);
	//}
	//else
	//{
	//	Blackboard->ClearValue(TEXT("PlayerLocation"));
	//}

}

bool AShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if(ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	return true; //NOTE happens when the pawn dies and detaches from the controller
}
