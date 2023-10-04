#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "HitInterface.h"
#include "DefenseTower.generated.h"

class UBoxComponent;

class UHealthComponent;
class UHealthBarWidget;

class AObjectPooling;

class UUnitComponent;

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

	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetObjectPooling(AObjectPooling* newObjectPooling) { objectPool = newObjectPooling; }

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

#pragma region Attack

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnHit(float damageAmount) override;
	UFUNCTION(BlueprintImplementableEvent)
		void OnTowerHit();
	UFUNCTION(BlueprintImplementableEvent)
		void OnTowerDestroyed();
	virtual bool OnDeath() override;
	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetHealthBarWidget(UHealthBarWidget* newHealthBarWidget) {healthBarWidget = newHealthBarWidget;}

#pragma endregion Attack

	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;
	virtual void LevelUpFunctionality() override;

private:

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnTowerDestroyedDelegateHandler();

#pragma region Combat

	void StartToAttack();
	void BackToUnocuppiedState();

	UPROPERTY(VisibleAnywhere,Category = "Combat")
		ETowerState towerState = ETowerState::ETS_Idle;

	UPROPERTY(VisibleAnywhere ,Category = "Combat")
		TArray<AActor*> overlapingEnemies;
	UPROPERTY(VisibleAnywhere ,Category = "Combat",BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		AActor* closestEnemy;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		UUnitComponent* targetUnitComponent;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UUnitComponent> unitComponentClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float attackRate;
	float projectileDamage;

	UPROPERTY()
		AObjectPooling* objectPool;

#pragma endregion Combat

	UPROPERTY()
		UHealthBarWidget* healthBarWidget;

#pragma region Components

	UPROPERTY(EditAnywhere)
		TArray<FUpgradingData> upgradeData;
	UPROPERTY(EditAnywhere)
		UBoxComponent* enemyDedectionCollider;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneComponent* projectileSpawnPoint;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UHealthComponent* healthComponent;

#pragma endregion Components

	FTimerHandle attackTimerRateHandle;
	FTimerHandle unocuppiedTimerStateHandle;
};
