#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveCompleteMulticastDelegate);

UCLASS()
class GRIDSYSTEM_API AUnitAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	FOnMoveCompleteMulticastDelegate OnMoveCompleteMDelegate;

protected:
	virtual void OnPossess(class APawn* InPawn) override;
};
