#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "DefenseTower.generated.h"

class UBoxComponent;
class AEnemyInfatry;
class AProjectile;
class UHealthComponent;

UENUM(BlueprintType)
enum class ETowerState : uint8
{
	ETS_Idle,
	ETS_Attacking,
	ETS_Unoccupied,

	ETS_NONE
};

UCLASS()
class GRIDSYSTEM_API ADefenseTower : public ABaseBuilding
{
	GENERATED_BODY()

public:
	ADefenseTower();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;
	virtual void LevelupFunctionality() override;
private:
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnTowerDestroyedDelegateHandler();

	void StartToAttack();
	void BackToUnocuppiedState();
	//void BackToAttack

	UPROPERTY(VisibleAnywhere,Category = "Tower State")
		ETowerState towerState = ETowerState::ETS_Idle;

	UPROPERTY(VisibleAnywhere ,Category = "Enemy")
		TArray<AEnemyInfatry*> overlapingEnemies;
	UPROPERTY(VisibleAnywhere ,Category = "Enemy")
	AEnemyInfatry* closestEnemy;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<AProjectile> projectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackRate;
	FTimerHandle attackTimerRateHandle;
	FTimerHandle unocuppiedTimerStateHandle;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMesh*> staticMeshs;
	UPROPERTY(EditAnywhere)
		UBoxComponent* enemyDedectionCollider;
	UPROPERTY(EditAnywhere)
		USceneComponent* projectileSpawnPoint;
	UPROPERTY(EditAnywhere)
		UHealthComponent* healthComponent;
};
