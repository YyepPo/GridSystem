#include "Buildings/ArcherGenerator.h"

#include "Units/UnitsGridComponent.h"
#include "UnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Grid/GridRepresentative.h"
#include "DataAssets/BuildingDataAsset.h"

AArcherGenerator::AArcherGenerator() :
	UnitsGridLocation{ CreateDefaultSubobject<USceneComponent>(FName(TEXT("Units Grid Location"))) },
	UnitsGridComponent { CreateDefaultSubobject<UUnitsGridComponent>(FName(TEXT("Units Grid Component"))) }
{
	UnitsGridLocation->SetupAttachment(GetRootComponent());
	UnitsGridComponent->SetComponentOwner(this);
}

void AArcherGenerator::BeginPlay()
{
	Super::BeginPlay();
	//Spawn the grid for units
	if (!bIsFriendlyBuilding && UnitsGridComponent)
	{
		UnitsGridComponent->SpawnUnitsGrid();
		GetWorld()->GetTimerManager().SetTimer(BuildingFunctionalityTimerHandle, this, &AArcherGenerator::BuildingFunctionalityTimer, loadedBuildingDataAsset->functionTimeRate, true);
	}

}

void AArcherGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UnitsGridComponent) UnitsGridComponent->SetComponentOwner(this);
}

void AArcherGenerator::OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	Super::OnBoxColliderClicked(TouchedComponent, ButtonPressed);
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Archer object is beign clicked")));
}

void AArcherGenerator::BuildingFunctionality()
{
	Super::BuildingFunctionality();
	//spawn units overtime
	if (UnitsGridComponent) UnitsGridComponent->SpawnUnitsGrid();
}

void AArcherGenerator::BuildingFunctionalityTimer()
{
	//if all grids are occupied, don't spawn any unit
	if (spawnedUnitCounter >= UnitsGridComponent->grids.Num()) 
	{ 
		//when all units are spawned attack the mother base

		//replace the player pawn with a MotherBuilding(its not yet created)
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		for (int32 i = 0; i < spawnedUnits.Num(); i++)
		{
			if (playerPawn) spawnedUnits[i]->MoveToTargetInterface(playerPawn);
		}
		//clear the FTimerHandle, so this function its called once  after all units are spawned 
		GetWorld()->GetTimerManager().ClearTimer(BuildingFunctionalityTimerHandle);
		return; 
	}

	if (GetWorld() && UnitToBeSpawned)
	{
		const FVector spawnLocation = GetActorLocation() + GetActorUpVector() * 100.f + GetActorForwardVector() * -150.f;
		AUnitBase* spawnedUnit = GetWorld()->SpawnActor<AUnitBase>(UnitToBeSpawned, spawnLocation, GetActorRotation());
		if (spawnedUnit && UnitsGridComponent)
		{
			spawnedUnits.AddUnique(spawnedUnit);
			for (int i = 0; i < UnitsGridComponent->grids.Num(); i++)
			{
				//check if grid at index i is not occupied,if its not then move the unit to that grid
				if (!UnitsGridComponent->grids[i]->IsOccupied())
				{
					spawnedUnit->Move(UnitsGridComponent->grids[i]->GetActorLocation());
					UnitsGridComponent->grids[i]->OccupyGrid();
					++spawnedUnitCounter;
					break;
				}
			}
		}
	}
}

void AArcherGenerator::LevelUpFunctionality()
{
	Super::LevelUpFunctionality();

	//on leveling up the building upgrade unitType to a stronger unitType
	switch (buildingLevelData.currentLevel)
	{
		case 2:	
			for (int32 i = 0; i < spawnedUnits.Num(); i++)
			{
				spawnedUnits[i]->Destroy();
			}
			spawnedUnitCounter = 0;
			UnitToBeSpawned = UnitToBeSpawnedLevel2;
			break;
	}
}

