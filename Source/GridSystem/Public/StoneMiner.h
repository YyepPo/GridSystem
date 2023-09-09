#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "StoneMiner.generated.h"

class UNiagaraComponent;
class AResourceBase;
UCLASS()
class GRIDSYSTEM_API AStoneMiner : public AUnitBase
{
	GENERATED_BODY()

public:
	AStoneMiner();
	virtual void Tick(float DeltaTime) override;

	void MoveToTarget(AActor* target) override;
	virtual void MoveToTargetInterface(AActor* target) override;

protected:
	virtual void BeginPlay() override;

	virtual void Attack() override;
	virtual void GainResource() override;
	void AttackRateTimer();

	bool CanResourceBeMined();

	void StartToRetreat();
	void Retreat();

private:
	void AttachWeaponToHand();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* weaponStaticMesh;

	//Retreat
	FVector startPosition;
	UPROPERTY(EditAnywhere, Category = "Retreat")
		float retreatRate;

	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trajectoryVFXComp;

	//Combat
	UPROPERTY(VisibleAnywhere,Category = "Mining")
		AResourceBase* currentResourceTarget;
	//resource
	bool bCanMoveToStoneResource = false;
	bool bCanMoveToWoodResource = false;

	FTimerHandle retreatTimerHandle;
	FTimerHandle attacktTimer;
};
