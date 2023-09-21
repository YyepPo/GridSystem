#include "UnitSpawner.h"

#include "NewUnit/NewEnemyUnit.h"

AUnitSpawner::AUnitSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUnitSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnWave();
}

void AUnitSpawner::SpawnWave()
{
	for (int i = 0; i < waveNumber; i++)
	{
		if(i == currentWave)
		{
			SpawnUnit(numberOfUnitsToBeSpawned[i]);
			currentWave++;
			UE_LOG(LogTemp, Warning, TEXT("Current wave is : %d"), currentWave);
			break;
		}
	}
}

void AUnitSpawner::SpawnUnit(int32 index)
{
	FVector spawnOffset(0, 0, 0);
	for (int i = 0; i < index; i++)
	{
		spawnOffset.X += 100;
		ANewEnemyUnit* spawnedUnit = GetWorld()->SpawnActor<ANewEnemyUnit>(enemyUnitClas, GetActorLocation() + spawnOffset, GetActorRotation());
		if(spawnedUnit)
		{
			aliveUnitNumber++;
			spawnedUnit->OnUnitDeathDelegate.AddDynamic(this, &AUnitSpawner::OnUnitDeathHandler);
		}
	}
}

void AUnitSpawner::OnUnitDeathHandler()
{
	aliveUnitNumber--;
	UE_LOG(LogTemp, Warning, TEXT("Current wave is : %d"), aliveUnitNumber);
	if(aliveUnitNumber == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(waveSpawnTimerHandle, this, &AUnitSpawner::SpawnWave, waveSpawnTimeRate);
	}
}

