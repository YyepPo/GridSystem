#include "PlayerController/GridPlayerController.h"
#include "HUD/MainHud.h"
#include "HUD/BuildingSelection.h"
void AGridPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//Get the hud
	HUD = Cast<AMainHud>(GetHUD());
	if (!HUD) { return; }

	//Add the necessary widgets to viewport
	HUD->SetUpBuildingSelectionHUD();
	
	//A reference to all widgets so the playerCharacter class can use it
	ShowBuildingReference = HUD->BuildingHUD->ShowSelectedBuildingWidgetClass;
	BuildingSelectionWidgetReference = HUD->BuildingHUD;
}
