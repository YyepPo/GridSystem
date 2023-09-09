#include "Buildings/BuildingsLevelUpComponent.h"

#include "BaseBuilding.h"

#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "HUD/BuildingLevelUpWidget.h"

UBuildingsLevelUpComponent::UBuildingsLevelUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuildingsLevelUpComponent::BeginPlay()
{
	Super::BeginPlay();

}
