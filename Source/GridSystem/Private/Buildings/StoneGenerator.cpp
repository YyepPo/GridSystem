#include "Buildings/StoneGenerator.h"

#include "Resource/Resource.h"

void AStoneGenerator::OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	Super::OnBoxColliderClicked(TouchedComponent, ButtonPressed);
}

void AStoneGenerator::BuildingFunctionality()
{
	Super::BuildingFunctionality();
}

void AStoneGenerator::BuildingFunctionalityTimer()
{
	Super::BuildingFunctionalityTimer();

	PopUpResourceWidget();
	if (resource) resource->AddStone(resourceAmount);
}

void AStoneGenerator::LevelupFunctionality()
{
	Super::LevelupFunctionality();
	LevelBasedResourceGain();
}
