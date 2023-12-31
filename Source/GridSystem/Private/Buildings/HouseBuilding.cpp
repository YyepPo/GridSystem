#include "Buildings/HouseBuilding.h"

#include "Resource/Resource.h"
void AHouseBuilding::BuildingFunctionality()
{
	Super::BuildingFunctionality();
}

void AHouseBuilding::BuildingFunctionalityTimer()
{
	Super::BuildingFunctionalityTimer();
	PopUpResourceWidget();
	if (resource) resource->AddPeople(resourceAmount);
}

void AHouseBuilding::LevelUpFunctionality()
{
	Super::LevelUpFunctionality();
	LevelBasedResourceGain();
}
