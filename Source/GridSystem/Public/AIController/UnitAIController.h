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
	AUnitAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	FOnMoveCompleteMulticastDelegate OnMoveCompleteMDelegate;

	FORCEINLINE void ShouldDisableMoveRequest(bool newShouldDisableMoveRequest) { bShouldDisableMoveRequest = newShouldDisableMoveRequest; }
protected:
	virtual void OnPossess(class APawn* InPawn) override;
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUnitComponent> unitComponentClass;
	bool bShouldDisableMoveRequest;
};
