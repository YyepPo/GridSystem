#pragma once

#include "CoreMinimal.h"
#include "Units/InfantryBase.h"
#include "Units/DeathInterface.h"
#include "Units/IsAttackableInterface.h"
#include "FriendlyInfantry.generated.h"

class AEnemyInfatry;

UCLASS()
class GRIDSYSTEM_API AFriendlyInfantry : public AInfantryBase, public IIsAttackableInterface
{
	GENERATED_BODY()
	
public:
	AFriendlyInfantry();
	virtual void Tick(float DeltaTime) override;
	virtual void IsAttackAble() override;
protected:
	virtual void BeginPlay() override;
	virtual void MoveToTarget(AActor* target) override;

	virtual void OnAttackDealDamage() override;
	virtual void StartToAttack() override;
	virtual bool IsTargetDead(IHitInterface* hitInterface) override;

	/// <summary>
	/// Combat
	/// </summary>
	virtual bool IsTargetValid();
	FTimerHandle attackTimerHandle;
	UPROPERTY(VisibleAnywhere)
		TArray<class AInfantryBase*> overlappingEnemyInfatries;
	UPROPERTY(VisibleAnywhere)
		AInfantryBase* enemyInfantryTarget;
private:
	UFUNCTION()
		void SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void SphereOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
