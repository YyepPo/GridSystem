#include "PlayerController/GridPlayerController.h"
#include "HUD/MainHud.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/BuildingSelection.h"
void AGridPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//Get the hud
	HUD = Cast<AMainHud>(GetHUD());
	if (!HUD) { return; }

	//Add the necessary widgets to viewport
	HUD->SetUpBuildingSelectioHUD();
	
	//A reference to all widgets so the playerCharacter class can use it
	ShowBuildingReference = HUD->BuildingHUD->ShowSelectedBuildingWidgetClass;
	if (ShowBuildingReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("PO"));
	}
	BuildingSelectionWidgetReference = HUD->BuildingHUD;
}
