#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "Infantry.generated.h"


UCLASS()
class GRIDSYSTEM_API AInfantry : public AUnitBase
{
	GENERATED_BODY()

public:
	AInfantry();
	virtual void Tick(float DeltaTime) override;

	void MoveToTarget(AActor* target) override;
protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack() override;
private:
	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
#pragma region Components

	UPROPERTY(EditAnywhere)
		class USphereComponent* sphereCollider;

#pragma endregion Components


	/// <summary>
	/// Combat
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		TArray<AInfantry*> overlapingActors;

	UPROPERTY(EditAnywhere, Category = "Combat")
		bool bIsUnitFriendly;
	AInfantry* currentEnemytarget;
	/// <summary>
	/// Health
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Health")
		float maxHealth;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		float currentHealth;
	bool bIsUnitDead = false;
public:
	void RemoveTargetFromOverlapingActors(AInfantry* infatryTarget);
	FORCEINLINE bool GetIsUnitFriendly() const { return bIsUnitFriendly; }
	FORCEINLINE bool GetIsUnitDead() const { return bIsUnitDead; }

};
