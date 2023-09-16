#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewUnit/UnitStates.h"
#include "UnitComponent.generated.h"

class AUnitAIController;
class UHealthComponent;
class UDecalComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UUnitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUnitComponent();
	void SetNewUnitAIController(AUnitAIController* AIController) { enemyAIController = AIController; }
	void SetAnimInstance(UAnimInstance* newAnimInstance) { animInstance = newAnimInstance; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void NewMove(FVector movePosition);

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
	void UpdateOverlapingTargets(AActor* target);
	FORCEINLINE void SetCurrentTarget(AActor* newCurrentTarget) { currentTarget = newCurrentTarget; }

#pragma endregion Combat

#pragma region Health

	FORCEINLINE bool GetUnitDead() const { return bDeadUnit; }
	void OnUnitHit(float damage);
	void OnUnitDeath();

#pragma endregion Health

protected:
	virtual void BeginPlay() override;
private:
	void PlayAMontage(UAnimMontage* montage);
	UFUNCTION()
		void OnMoveCompleteDelegateHandler();

#pragma region Combat
	bool IsUnitInAttackRangeOf(AActor* target);
	bool CanUnitAttack(AActor* target);
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
	UHealthComponent* targetHealthComponent;
	TSubclassOf<UUnitComponent> unitComponentClass;
	FTimerHandle attackRateTimerHandle;

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

#pragma region UnitSelection

	UFUNCTION()
		void SetOverlayMaterialEmmision(float emmisionAmount);
	UPROPERTY()
		UDecalComponent* ownersDecalComponent;
	UPROPERTY(EditAnywhere)
		bool bIsUnitSelectable;
	UPROPERTY(VisibleAnywhere)
		bool bIsUnitSelected;

	UMaterialInstanceDynamic* dynamicMaterialInstance;
#pragma endregion UnitSelection

	AUnitAIController* enemyAIController;
	UAnimInstance* animInstance;
	//units current destination, when movecomplete has finished unit rotates toward destination variable
	FVector destination;
};
