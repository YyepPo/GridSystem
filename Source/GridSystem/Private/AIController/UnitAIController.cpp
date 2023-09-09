#include "AIController/UnitAIController.h"

#include "UnitBase.h"
void AUnitAIController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AUnitBase* pawn = Cast<AUnitBase>(InPawn);
	if (pawn)
	{
		pawn->SetAIController(this);
	}
}

void AUnitAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	OnMoveCompleteMDelegate.Broadcast();
}

