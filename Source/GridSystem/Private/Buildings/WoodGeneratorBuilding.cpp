#include "Buildings/WoodGeneratorBuilding.h"
#include "Resource/Resource.h"

void AWoodGeneratorBuilding::OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	Super::OnBoxColliderClicked(TouchedComponent, ButtonPressed);
}

void AWoodGeneratorBuilding::BuildingFunctionality()
{
	Super::BuildingFunctionality();
}

void AWoodGeneratorBuilding::BuildingFunctionalityTimer()
{
	Super::BuildingFunctionalityTimer();
	if (!resource) { return; }

	resource->AddWood(resourceAmount);
}

void AWoodGeneratorBuilding::LevelupFunctionality()
{
	Super::LevelupFunctionality();
	LevelBasedResourceGain();
}

