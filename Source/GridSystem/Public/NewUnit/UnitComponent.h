#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewUnit/UnitStates.h"
#include "NewUnit/TypeUnit.h"
#include "HitInterface.h"
#include "UnitComponent.generated.h"

class AUnitAIController;
class UHealthComponent;
class UDecalComponent;
class AProjectile;
class AObjectPooling;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UUnitComponent : public UActorComponent,public IHitInterface
{
	GENERATED_BODY()

public:
	UUnitComponent();
	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetObjectPool(AObjectPooling* newPoolObject) { objectPooling = newPoolObject; }
	FORCEINLINE void SetNewUnitAIController(AUnitAIController* AIController) { enemyAIController = AIController; }
	FORCEINLINE void SetAnimInstance(UAnimInstance* newAnimInstance) { animInstance = newAnimInstance; }
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void NewMove(FVector movePosition);

	virtual void OnHit(float damageAmount) override;
	virtual bool OnDeath() override;

#pragma region UnitSelection

	//Getters and setters
	FORCEINLINE void SetDynamicMaterialInstance(UMaterialInstanceDynamic* newDynamicMaterialInstance) { dynamicMaterialInstance = newDynamicMaterialInstance; }
	FORCEINLINE void SetSelectionDecalComponent(UDecalComponent* newDecalComponent) { ownersDecalComponent = newDecalComponent; }
	FORCEINLINE void SetIsUnitSelected(bool bIsSelected);
	FORCEINLINE bool GetIsUnitSelected() const { return bIsUnitSelected; }

#pragma endregion UnitSelection

#pragma region Combat

	UFUNCTION(BlueprintCallable)
		void DealDamageToTargetAnimNotify();//Used to deal damage in animation notify
	void UpdateOverlapingTargets(AActor* target);//Used in NewFriendlyClass and NewEnemyClass,to update current target 
	FORCEINLINE void SetCurrentTarget(AActor* newCurrentTarget) { currentTarget = newCurrentTarget; }

#pragma endregion Combat

#pragma region Health
		
	void OnUnitHit(float damage);
	void OnUnitDeath();

	FORCEINLINE float GetUnitHealthAmount() const { return healthAmount; }
	FORCEINLINE bool GetUnitDead() const { return bDeadUnit; }

#pragma endregion Health

protected:
	virtual void BeginPlay() override;
private:
	void PlayAMontage(UAnimMontage* montage) const;
	UFUNCTION()
		void OnMoveCompleteDelegateHandler();

#pragma region Combat

	bool IsUnitInAttackRangeOf(AActor* target) const;
	bool CanUnitAttack(AActor* target) const;
	void AttackBehaviour();
	void Attack();
	void AttackRateHandle();
	void CheckForClosestTarget();

	UPROPERTY(EditAnywhere, Category = "Combat")
		UAnimMontage* attackMontage;
	UPROPERTY(VisibleAnywhere,Category = "Combat")
		AActor* currentTarget;
	UPROPERTY(VisibleAnywhere,Category = "Combat")
		TArray<AActor*> overlapingTargets;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackDistance;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackRate;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackDamage;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackOffset;
	UPROPERTY(EditAnywhere, Category = "Combat|Archer")
		TSubclassOf<AProjectile> projectileClass;
	UPROPERTY(EditAnywhere, Category = "Combat|Archer")
		float projectileDamage;
	UPROPERTY()
		UHealthComponent* targetHealthComponent;
	TSubclassOf<UUnitComponent> unitComponentClass;
	FTimerHandle attackRateTimerHandle;

	UPROPERTY()
		AObjectPooling* objectPooling;
#pragma endregion Combat

#pragma region Health

	UPROPERTY(EditAnywhere, Category = "Health")
		UAnimMontage* hitMontage;		
	UPROPERTY(EditAnywhere, Category = "Health")
		UAnimMontage* deathMontage;		
	UPROPERTY(EditAnywhere, Category = "Health")
		float healthAmount;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		bool bDeadUnit; //is unit dead or not
#pragma endregion Health

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UnitStates unitState = UnitStates::EUS_Idle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		ETypeUnit unitType = ETypeUnit::ETU_NONE;

#pragma region UnitSelection

	UFUNCTION()
		void SetOverlayMaterialEmmision(float emmisionAmount);
	UPROPERTY()
		UDecalComponent* ownersDecalComponent;
	UPROPERTY(EditAnywhere)
		bool bIsUnitSelectable;
	UPROPERTY(VisibleAnywhere)
		bool bIsUnitSelected;

	UPROPERTY()
		UMaterialInstanceDynamic* dynamicMaterialInstance;

#pragma endregion UnitSelection

	UPROPERTY()
		AUnitAIController* enemyAIController;
	UPROPERTY()
		UAnimInstance* animInstance;
	//units current destination, when movecomplete has finished unit rotates towards destination variable
	FVector destination;

	UPROPERTY(EditAnywhere)
		USceneComponent* projectileSpawnPositionComp;
};
