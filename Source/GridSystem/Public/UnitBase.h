#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Units/MoveToTargetInterface.h"
#include "UnitBase.generated.h"

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	EUS_Idle UMETA(DisplayName = "Idle"),
	EUS_Attacking UMETA(DisplayName = "Attacking"),
	EUS_UnOcuppied UMETA(DisplayName = "UnOcuppied"),
	EUS_Dead UMETA(DisplayName = "Dead"),
	EUS_NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EunitType : uint8
{
	EUT_WoodChoper UMETA(DisplayName = "WoodChoper"),
	EUT_StoneMiner UMETA(DisplayName = "StoneMiner"),
	EUT_Archer UMETA(DisplayName = "Archer"),
	EUT_Infantry UMETA(DisplayName = "Infantry"),
	EUT_Spear UMETA(DisplayName = "Spear"),
	EUT_Magician UMETA(DisplayName = "Magician"),

	EUT_NONE UMETA(DisplayName = "None")
};

class AAIController;
class AUnitAIController;

UCLASS()
class GRIDSYSTEM_API AUnitBase : public ACharacter,public IMoveToTargetInterface 
{
	GENERATED_BODY()

public:
	AUnitBase();
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnUnitSelected(bool isSelected);

	virtual void Move(FVector MovePosition);
	virtual void MoveToTarget(AActor* target);
	virtual void MoveToTargetInterface(AActor* target) override;

	void SetAIController(AUnitAIController* AIController) { enemyAIController = AIController; }

	FORCEINLINE bool GetIsSelected() const { return bIsSelected; }
	FORCEINLINE bool IsUnitDead() const { return isDead == true; }

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Unit Type")
		EunitType unitType = EunitType::EUT_NONE;

	void SetRotationTowardsTarget(AActor* target);

	//Unit Selection
	UFUNCTION()
		void SetOverlayMaterialEmmision(float emmisionAmount);
	UPROPERTY(EditAnywhere, Category = "Decal")
		class UDecalComponent* DecalComponent;
	UPROPERTY(EditAnywhere, Category = "Decal")
		bool bIsSelectable = true;
	bool bIsSelected;

	/// <summary>
	/// Combat
	/// </summary>
	virtual void Attack();
	virtual bool IsInAttackRange(AActor* target);
	virtual bool IsAttacking();
	virtual void PlayAMontage(UAnimMontage* montage);
	virtual void PlayAMontageWithSection(UAnimMontage* montage, FName sectionName);
	UFUNCTION(BlueprintCallable)
		void GainResourceOnAttack();
	UFUNCTION(BlueprintCallable)
		void SetUnitToUnOcuppiedState();
	virtual void GainResource();

	UPROPERTY(EditAnywhere, Category = "AI")
		bool bIsEnemyUnit = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"), Category = "AI")
		EUnitState unitState = EUnitState::EUS_NONE;
	UPROPERTY(EditAnywhere, Category = "AI")
		UAnimMontage* attackMontage;
	UPROPERTY(EditAnywhere, Category = "AI")
		float navigationAcceptableRadius;
	UPROPERTY(EditAnywhere, Category = "AI")
		float attackDistance;
	UPROPERTY(EditAnywhere, Category = "AI")
		float attackRate;
	UPROPERTY(EditAnywhere, Category = "AI")
		float attackRadius;
	UPROPERTY(EditAnywhere, Category = "AI")
		float attackDamage;
	UPROPERTY(VisibleAnywhere, Category = "AI",BlueprintReadWrite,meta = (AllowPritvateAccess = "true"))
		bool isDead;

	UPROPERTY()
		AAIController* aiController;
	UPROPERTY()
		AUnitAIController* enemyAIController;
	UPROPERTY()
		AActor* currentTarget;

	UFUNCTION()
		void OnMoveCompleteDelegateHandler();
	UPROPERTY(EditAnywhere,Category = "Rotation")
		float rotationInterpSpeed;
	UPROPERTY(EditAnywhere,Category = "Rotation")
		float rate;
	bool bHasReachedDestination;
	FVector targetPosition;
		
	FTimerHandle testHandle;
	void TimerHandleTest();
};
