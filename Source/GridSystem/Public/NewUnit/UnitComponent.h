#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitComponent.generated.h"

class AUnitAIController;

UENUM(BlueprintType)
enum class EUnitsState : uint8
{
	EUS_Idle UMETA(DisplayName = "Idle"),
	EUS_Attacking UMETA(DisplayName = "Attacking"),
	EUS_UnOcuppied UMETA(DisplayName = "UnOcuppied"),
	EUS_Dead UMETA(DisplayName = "Dead"),
	EUS_NONE UMETA(DisplayName = "None")
};

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
	void SetIsUnitSelected(bool bIsSelected);
	bool GetIsUnitSelected() const { return bIsUnitSelected; }
#pragma endregion UnitSelection

#pragma region Combat

	FORCEINLINE void SetCurrentTarget(AActor* newCurrentTarget) { currentTarget = newCurrentTarget; }

#pragma endregion Combat

protected:
	virtual void BeginPlay() override;

private:
	void PlayAMontage(UAnimMontage* montage);

#pragma region Combat

	bool IsUnitInAttackRangeOf(AActor* target);
	bool CanUnitAttack(AActor* target);
	void Attack();
	void AttackRateHandle();

	UPROPERTY(VisibleAnywhere,Category = "Combat")
		AActor* currentTarget;
	UPROPERTY(EditAnywhere,Category = "Combat")
		UAnimMontage* attackMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackDistance;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackRate;

	FTimerHandle attackRateTimerHandle;

#pragma endregion Combat

	UPROPERTY(VisibleAnywhere)
		EUnitsState unitState = EUnitsState::EUS_Idle;

#pragma region UnitSelection

	UPROPERTY(EditAnywhere)
		bool bIsUnitSelectable;
	UPROPERTY(VisibleAnywhere)
		bool bIsUnitSelected;

#pragma endregion UnitSelection

		
	AUnitAIController* enemyAIController;
	UAnimInstance* animInstance;
};
