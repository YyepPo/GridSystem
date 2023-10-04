#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralObjectGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRandomSpawnLocationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float minSpawnLocationX;
	UPROPERTY(EditAnywhere)
		float maxSpawnLocationX;

	UPROPERTY(EditAnywhere)
		float minSpawnLocationY;
	UPROPERTY(EditAnywhere)
		float maxSpawnLocationY;
};

UCLASS()
class GRIDSYSTEM_API AProceduralObjectGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AProceduralObjectGenerator();

protected:

	virtual void BeginPlay() override;

private:

	void SpawnObjectsInRandomLocation() const;
	float GenerateRandomXLocation() const;
	float GenerateRandomYLocation() const;

	bool HasLineTraceHit(FHitResult& hitResult,FVector spawnLocation,FVector endLocation) const;
	bool HasSweepHit(FHitResult& sweepResult, FVector spawnPoint) const;

	UPROPERTY(EditAnywhere)
		float offSetZ;

	UPROPERTY(EditAnywhere)
		float lineTraceLength;
	UPROPERTY(EditAnywhere)
		float sphereRadius;

	UPROPERTY(EditAnywhere)
		bool bDebugEnabled = false;

	UPROPERTY(EditAnywhere)
		FRandomSpawnLocationData spawnLocationData;

	UPROPERTY(EditAnywhere)
		int32 numberOfObjectsToSpawn;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> objectClass;
};
