#include "ProceduralGeneration/ProceduralObjectGenerator.h"

#include "DrawDebugHelpers.h"

AProceduralObjectGenerator::AProceduralObjectGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AProceduralObjectGenerator::BeginPlay()
{
	Super::BeginPlay();

	SpawnObjectsInRandomLocation();
}

void AProceduralObjectGenerator::SpawnObjectsInRandomLocation() const
{
	for (int i = 0; i < numberOfObjectsToSpawn; i++)
	{
		if (!objectClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Object class is not selected for procedural object generator"));
			break;
		}

		const FVector spawnLocation = FVector(GenerateRandomXLocation(), GenerateRandomYLocation(), offSetZ);
		//downwards direction
		const FVector endLocation = spawnLocation + GetActorUpVector() * -lineTraceLength;

		if (bDebugEnabled) DrawDebugLine(GetWorld(), spawnLocation, endLocation, FColor::Red, false, 20.f, 0, 2.f);

		//Line trace in downwards direction to detect terrain,if trace didn't hit the terrain then the trace is out of bounds
		FHitResult hitResult;
		if (!HasLineTraceHit(hitResult, spawnLocation, endLocation))
		{
			UE_LOG(LogTemp, Warning, TEXT("Line trace is out of bounds"));
			continue;
		}

		if (bDebugEnabled) DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, sphereRadius, 8, FColor::Blue, true, 10.f);

		//Store the hitResultImpactPoint so later on if sweepResult didn't hit, spawn the object on hitResultImpactPoint's location
		FVector objectSpawnPoint = hitResult.ImpactPoint;

		//If sweepResult is not true which means that there is no close object to the ActorClass that is going to be spawned,so if there is no close object spawn the ActorClass
		FHitResult sweepResult;
		if (HasSweepHit(sweepResult, hitResult.ImpactPoint))
		{
			//If sweepResult is true which means that there is a close object to the ActorClass that is going to be spawned,so if there is a close object don't spawn the ActorClass
			//Iterate for x amount,generate random locations and check if there is any close object on generated random location until there is no close objects,
			//and when there is no any close object spawn the ActorClass
			for (int k = 0; k < 5; k++)
			{
				FVector newRandomLoc(GenerateRandomXLocation(), GenerateRandomYLocation(), hitResult.ImpactPoint.Z);

				if (bDebugEnabled) DrawDebugSphere(GetWorld(), newRandomLoc, sphereRadius, 8, FColor::Red, true, 10.f);

				FHitResult newSweepResult;
				if (HasSweepHit(newSweepResult, newRandomLoc))
				{
					continue;
				}

				GetWorld()->SpawnActor<AActor>(objectClass, newRandomLoc, GetActorRotation());
				break;
			}

			UE_LOG(LogTemp, Warning, TEXT("There is a close object,dont spawn the Actor class"));
			continue;
		}

		GetWorld()->SpawnActor<AActor>(objectClass, objectSpawnPoint, GetActorRotation());
	}


}

float AProceduralObjectGenerator::GenerateRandomXLocation() const
{
	return FMath::RandRange(spawnLocationData.minSpawnLocationX,spawnLocationData.maxSpawnLocationX);
}

float AProceduralObjectGenerator::GenerateRandomYLocation() const
{
	return 	FMath::RandRange(spawnLocationData.minSpawnLocationY, spawnLocationData.maxSpawnLocationY);
}

bool AProceduralObjectGenerator::HasLineTraceHit(FHitResult& hitResult,FVector spawnLocation,FVector endLocation) const
{
	return GetWorld()->LineTraceSingleByChannel(hitResult, spawnLocation, endLocation, ECollisionChannel::ECC_Visibility);
}

bool AProceduralObjectGenerator::HasSweepHit(FHitResult& sweepResult,FVector spawnPoint) const
{
	const FCollisionShape sphereShape = FCollisionShape::MakeSphere(sphereRadius);
	return GetWorld()->SweepSingleByChannel(sweepResult,
		spawnPoint,
		spawnPoint,
		FQuat::Identity,
		ECC_GameTraceChannel7,
		sphereShape);
}
