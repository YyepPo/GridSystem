
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawner.generated.h"

class ANewEnemyUnit;

UCLASS()
class GRIDSYSTEM_API AUnitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnitSpawner();

protected:
	virtual void BeginPlay() override;

private:

	void SpawnWave();
	void SpawnUnit(int32 index);
	UFUNCTION()
		void OnUnitDeathHandler();
	
	UPROPERTY(EditAnywhere, Category = "Units")
		TSubclassOf<ANewEnemyUnit> enemyUnitClas;
	UPROPERTY(VisibleAnywhere, Category = "Units")
		int32 aliveUnitNumber;
	UPROPERTY(EditAnywhere, Category = "Units")
		TArray<int32> numberOfUnitsToBeSpawned;
	UPROPERTY(EditAnywhere, Category = "Units")
		float waveSpawnTimeRate;

	FTimerHandle waveSpawnTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Wave")
		int32 currentWave = 1;
	UPROPERTY(EditAnywhere, Category = "Wave")
		int32 waveNumber;


};
