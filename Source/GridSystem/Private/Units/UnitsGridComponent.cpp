#include "Units/UnitsGridComponent.h"

#include "Grid/GridRepresentative.h"
#include "Buildings/ArcherGenerator.h"

#include "Components/SceneComponent.h"


UUnitsGridComponent::UUnitsGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UUnitsGridComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUnitsGridComponent::SpawnUnitsGrid()
{
	FVector SpawnLocation = FVector();
	if (ArcherBuilding) SpawnLocation = ArcherBuilding->UnitsGridLocation->GetComponentLocation();
	SpawnLocation.Z = 0;

	for (int i = 0; i < Rows; i++)
	{
		SpawnLocation.X += i * CellSize;
		for (int k = 0; k < Columns; k++)
		{
			SpawnLocation.Y += k * CellSize;
			if (GetWorld() && GridRepresentativeClass)
			{
				AGridRepresentative* spawnedGrid = GetWorld()->SpawnActor<AGridRepresentative>(GridRepresentativeClass, SpawnLocation, GetOwner()->GetActorRotation());
				if (spawnedGrid) grids.AddUnique(spawnedGrid);
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Total units grid numbet is: %d"), grids.Num()));
}
