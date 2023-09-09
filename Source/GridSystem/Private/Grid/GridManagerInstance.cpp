#include "Grid/GridManagerInstance.h"
#include "Grid/GridRepresentative.h"

void UGridManagerInstance::SpawnGrids()
{
	//Store the spawn location
	FVector SpawnLocation = FVector();
	//Set z axis to 0 so the grid doesnt float
	SpawnLocation.Z = zOffset;

	//Loop through Rows
	for (int i = 0; i < Rows; i++)
	{
		//Calculate the X component of the spawn location based on the current row and the cell size.
		SpawnLocation.X = i * CellSize;

		//Loop through Columns
		for (int k = 0; k < Columns; k++)
		{
			//Calculate the Y component of the spawn location based on the current row and the cell size.
			SpawnLocation.Y = k * CellSize;

			//Check if GetWorld() && GridRepresentativeClass(TSubclass) are valid
			if (GetWorld() && gridRepresentativeClass)
			{
				//Spawn GridRepresentativeClass at the calculated SpawnLocation
				GetWorld()->SpawnActor<AGridRepresentative>(gridRepresentativeClass, SpawnLocation,FRotator(0,0,0));
			}
		}
	}
}
