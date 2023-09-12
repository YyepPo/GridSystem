#include "AIController/UnitAIController.h"

#include "UnitBase.h"

#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"
AUnitAIController::AUnitAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUnitAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UUnitComponent* unitComp = Cast<UUnitComponent>(InPawn->GetComponentByClass(unitComponentClass));
	if (unitComp)
	{
		unitComp->SetNewUnitAIController(this);
	}
	
	
	
	AUnitBase* pawn = Cast<AUnitBase>(InPawn);
	if (pawn)
	{
		pawn->SetAIController(this);
	}
}

void AUnitAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldDisableMoveRequest)
	{
		FAIRequestID requestID;
		requestID.InvalidRequest;
		FPathFollowingResult folow(FPathFollowingResultFlags::MovementStop);
		if (folow.HasFlag(FPathFollowingResultFlags::MovementStop))
			UE_LOG(LogTemp, Warning, TEXT("AA"));
		OnMoveCompleted(requestID,folow);
	}
}

void AUnitAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	OnMoveCompleteMDelegate.Broadcast();
}

