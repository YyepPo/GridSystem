#include "Buildings/ArcherGenerator.h"

#include "Units/UnitsGridComponent.h"
#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Grid/GridRepresentative.h"
#include "DataAssets/BuildingDataAsset.h"

////
//// This class if for spawning units like (archer,infantry etc);
////

AArcherGenerator::AArcherGenerator() :
	UnitsGridLocation{ CreateDefaultSubobject<USceneComponent>(FName(TEXT("Units Grid Location"))) },
	UnitsGridComponent { CreateDefaultSubobject<UUnitsGridComponent>(FName(TEXT("Units Grid Component"))) }
{
	UnitsGridLocation->SetupAttachment(GetRootComponent());
	UnitsGridComponent->SetComponentOwner(this);
	bOccupiesNeighbours = true;
	if (units.Num() != 0) unitToBeSpawned = units[0];
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

	UE_LOG(LogTemp, Warning, TEXT("%d level"), buildingLevelData.currentLevel);

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
	OccupyNeighbours();
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
		//APawn* playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		//for (int32 i = 0; i < spawnedUnits.Num(); i++)
		//{
		//	if (playerPawn) spawnedUnits[i]->GetUnitsUnitComponent()->NewMove(playerPawn->GetActorLocation());
		//}
		//clear the FTimerHandle, so this function its called once  after all units are spawned 
		GetWorld()->GetTimerManager().ClearTimer(BuildingFunctionalityTimerHandle);
		return; 
	}

	if (GetWorld() && unitToBeSpawned)
	{
		const FVector spawnLocation = GetActorLocation() + GetActorUpVector() * 100.f + GetActorForwardVector() * -150.f;
		ANewFriendlyUnit* spawnedUnit = GetWorld()->SpawnActor<ANewFriendlyUnit>(unitToBeSpawned, spawnLocation, GetActorRotation());
		if (spawnedUnit && UnitsGridComponent)
		{
			spawnedUnits.AddUnique(spawnedUnit);
			for (int i = 0; i < UnitsGridComponent->grids.Num(); i++)
			{
				//check if grid at index i is not occupied,if its not then move the unit to that grid
				if (!UnitsGridComponent->grids[i]->IsOccupied())
				{
					if(spawnedUnit && spawnedUnit->GetUnitsUnitComponent())
					{
						spawnedUnit->GetUnitsUnitComponent()->NewMove(UnitsGridComponent->grids[i]->GetActorLocation());
						UnitsGridComponent->grids[i]->OccupyGrid();
						++spawnedUnitCounter;
					}
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

	for (int32 k = 0; k < spawnedUnits.Num(); k++)
	{
		spawnedUnits[k]->Destroy();
	}
	spawnedUnitCounter = 0;

	for (int i = 0; i < buildingLevelData.maxLevel; ++i)
	{
		if(buildingLevelData.currentLevel == i)
		{
			unitToBeSpawned = units[i];
			UE_LOG(LogTemp, Warning, TEXT("Same level"));
			break;
		}
	}

	for (int i = 0; i < UnitsGridComponent->grids.Num(); i++)
	{
		UnitsGridComponent->grids[i]->UnOccupyGrid();
	}

}

