#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "HitInterface.h"
#include "InfantryBase.generated.h"

// this is used for death animations
UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Death1 UMETA(DisplayName = "Death 1"),
	EDP_Death2 UMETA(DisplayName = "Death 2")
};


UCLASS()
class GRIDSYSTEM_API AInfantryBase : public AUnitBase
{
	GENERATED_BODY()
	
public:
	AInfantryBase();

	virtual void Tick(float DeltaTime) override;

	void MoveToTarget(AActor* target) override;

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// Combat
	/// </summary>
	virtual bool IsTargetDead(IHitInterface* hitInterface);
	bool CanAttack(AActor* target);
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnAttackDealDamage(); //used to be overriden by child class,is used in OnAttackDealDamageAnimNotify()
	void DamageBehaviour(AActor* target);
	virtual void StartToAttack();

	/// <summary>
	/// Health
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Health")
		float maxHealth;
	UPROPERTY(EditAnywhere, Category = "Health")
		UAnimMontage* hitMontage;
	UPROPERTY(EditAnywhere, Category = "Health")
		UAnimMontage* deathMontage;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		float currentHealthAmount;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		bool bIsUnitDead;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float sphereRadiuss;

#pragma region Components

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class USphereComponent* sphereCollider;

#pragma endregion Components

	/// <summary>
	/// Overlapping
	/// </summary>

	void OnCollisionBehaviour(AInfantryBase& actor, TArray<AInfantryBase*>& actors, AInfantryBase* targetUnit);
	void CheckForClosestTargett(AInfantryBase*& unitTarget, TArray<AInfantryBase*>& overlapingUnits);

private:
	// Overlapping Functions
	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// Combat
	/// </summary>
	bool DedectTarget(FHitResult& hitResult);
	UFUNCTION(BlueprintCallable)
		void OnAttackDealDamageAnimNotify();//used for animation notify
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EDeathPose deathPose;




};
