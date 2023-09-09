#include "HUD/BuildingSelection.h"
#include "HUD/BuildingWidget.h"

#include "Components/WrapBox.h"

void UBuildingSelection::NativeConstruct()
{
	Super::NativeConstruct();
	//Building selection widget 
	//A small inventory
	if (WrapBox)
	{
		WrapBox->AddChild(Slot1);
		WrapBox->AddChild(Slot2);
		WrapBox->AddChild(Slot3);
		WrapBox->AddChild(Slot4);
		WrapBox->AddChild(Slot5);
	}
}