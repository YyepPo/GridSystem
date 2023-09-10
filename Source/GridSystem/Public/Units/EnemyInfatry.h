#pragma once

#include "CoreMinimal.h"
#include "Units/InfantryBase.h"
#include "Units/IsAttackableInterface.h"
#include "EnemyInfatry.generated.h"

class ADefenseTower;

UCLASS()
class GRIDSYSTEM_API AEnemyInfatry : public AInfantryBase, public IIsAttackableInterface
{
	GENERATED_BODY()
	
public:
	AEnemyInfatry();
	virtual void Tick(float DeltaTime) override;

	virtual void IsAttackAble() override;

protected:
	virtual void BeginPlay() override;

	virtual void MoveToTarget(AActor* target) override;
	virtual void OnAttackDealDamage() override;
	virtual void StartToAttack() override;
	virtual bool IsTargetDead(IHitInterface* hitInterface) override;

private:
	UFUNCTION()
		void SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SphereOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FTimerHandle attackTimerHandle;
	/// <summary>
	/// Combat
	/// </summary>
	bool IsTargetValid();
	bool IsTowerTargetValid();
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		TArray<class AInfantryBase*> overlapingInfantry;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		AInfantryBase* currentFriendlyUnitTarget;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		TArray<AActor*> overlapingActors;
	bool IsInAttackRangeOfActor(AActor* actor);

	bool DedectTarget(FHitResult& hitResult);

		UPROPERTY(EditAnywhere,Category = "Combat")
	ADefenseTower* towerTarget;
};
