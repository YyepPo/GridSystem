#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "HitInterface.h"
#include "DefenseTower.generated.h"

class UBoxComponent;
class AEnemyInfatry;
class AProjectile;
class UHealthComponent;
class UHealthBarWidget;

UENUM(BlueprintType)
enum class ETowerState : uint8
{
	ETS_Idle,
	ETS_Attacking,
	ETS_Unoccupied,

	ETS_NONE
};

USTRUCT(BlueprintType)
struct FUpgradingData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		UStaticMesh* mesh;
	UPROPERTY(EditAnywhere)
		float damage;
};

UCLASS()
class GRIDSYSTEM_API ADefenseTower : public ABaseBuilding, public IHitInterface
{
	GENERATED_BODY()

public:
	ADefenseTower();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnHit(float damageAmount) override;
	UFUNCTION(BlueprintImplementableEvent)
		void OnTowerHit();
	UFUNCTION(BlueprintImplementableEvent)
		void OnTowerDestroyed();
	virtual bool OnDeath() override;

	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;
	virtual void LevelUpFunctionality() override;

	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetHealthBarWidget(UHealthBarWidget* newHealthBarWidget) {healthBarWidget = newHealthBarWidget;}
private:
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnTowerDestroyedDelegateHandler();

	void StartToAttack();
	void BackToUnocuppiedState();

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
	float projectileDamage;

	UPROPERTY()
		UHealthBarWidget* healthBarWidget;

	UPROPERTY(EditAnywhere)
		TArray<FUpgradingData> upgradeData;
	UPROPERTY(EditAnywhere)
		UBoxComponent* enemyDedectionCollider;
	UPROPERTY(EditAnywhere)
		USceneComponent* projectileSpawnPoint;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UHealthComponent* healthComponent;

	FTimerHandle attackTimerRateHandle;
	FTimerHandle unocuppiedTimerStateHandle;
};
