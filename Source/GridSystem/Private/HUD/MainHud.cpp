#include "HUD/MainHud.h"
#include "HUD/BuildingSelection.h"
#include "HUD/ResourceWidget.h"
#include "HUD/ShowBuilding.h"

#include "UnitBase.h"
#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Blueprint/UserWidget.h"

void AMainHud::BeginPlay()
{
	Super::BeginPlay();
}

void AMainHud::DrawHUD()
{
	Super::DrawHUD();
	
	UnitSelectionBehaviour();
}

void AMainHud::SetUpBuildingSelectioHUD()
{
	//Add the buildingSelectionWidget to viewport if exists
	playerController = (playerController == nullptr) ? GetOwningPlayerController() : playerController;
	if (!playerController) { return; }

	BuildingHUD = CreateWidget<UBuildingSelection>(playerController, BuildingHUDClass);
	if (BuildingHUD) BuildingHUD->AddToViewport();

}

void AMainHud::SetUpResourceWidget()
{
	//Add the ResourceWidget to viewport if exists
	//playerController = (playerController == nullptr) ? GetOwningPlayerController() : playerController;
	//if (!playerController) { return; }
	//
	//ResourceWidget = CreateWidget<UResourceWidget>(playerController, ResourceWidgetClass);
	//if (ResourceWidget) ResourceWidget->AddToViewport();
}

void AMainHud::SetUpShowBuildingWidget()
{
	//Add the ShowBuildingWidget to viewport if exists
	playerController = (playerController == nullptr) ? GetOwningPlayerController() : playerController;
	if (!playerController) { return; }

	ShowBuildingWidget = CreateWidget<UShowBuilding>(playerController, ShowBuildingWidgetClass);
	if (ShowBuildingWidget) ShowBuildingWidget->AddToViewport();
}

void AMainHud::UnitSelectionBehaviour()
{
	if (!bIsDrawing) { return; }

	DrawRectangle();

	OnUnitSelection();
}

void AMainHud::DrawRectangle()
{
	//Get mouse position in screen coordinates
	GetOwningPlayerController()->GetMousePosition(mouseEndPosition.X, mouseEndPosition.Y);
	// Calculate the width and height of the selection rectangle.
	const float screenWidth = mouseEndPosition.X - mouseStartPosition.X;
	const float screenHeight = mouseEndPosition.Y - mouseStartPosition.Y;
	/* Draw a rectangle on the screen based on mouse position
	   *mouseStartPosition.X* & *mouseStartPosition.Y* defines the top right corner */
	//*screenWidth* & *screenHeight* determines width & height of the rectangle
	DrawRect(RectangleSelectionColor, mouseStartPosition.X, mouseStartPosition.Y, screenWidth, screenHeight);
}

void AMainHud::OnUnitSelection()
{
	//Get all actors in SelectionRectangle
	if (!UnitBaseClass) { return; }
	TArray<AActor*> actors;
	//GetActorsInSelectionRectangle(mouseStartPosition, mouseEndPosition, selectedActors, false, true);
	GetActorsInSelectionRectangle(mouseStartPosition, mouseEndPosition, actors, false, true);
	//Loop through all selectedActors
	for (int i = 0; i < actors.Num(); i++)
	{
		//Cast each selectedActor to an UnitBase (selectedUnit)
		//AUnitBase* selectedUnit = Cast<AUnitBase>(selectedActors[i]);
		UUnitComponent* comp = Cast<UUnitComponent>(actors[i]->GetComponentByClass(unitComponentClass));
		if (comp)
		{
			unitComponents.AddUnique(comp);
			comp->SetIsUnitSelected(true);
			UE_LOG(LogTemp, Warning, TEXT("comp is ava"));
		}

		for (int32 k = 0; k < unitComponents.Num(); k++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"),*unitComponents[k]->GetOwner()->GetActorNameOrLabel());
		}
		//if (selectedUnit)
		//{
		//	//Add the selected unit to an array of AUnitBase
		//	selectedUnits.AddUnique(selectedUnit);
		//	//And select that unit (on unit selection a circle is begin draw to inform the player when that unit is selected)
		//	selectedUnit->OnUnitSelected(true);
		//}
	}

}


void AMainHud::StartDraw()
{
	//On startDraw deselect the units that are selected and then start drawing
	bIsDrawing = true;
	GetOwningPlayerController()->GetMousePosition(mouseStartPosition.X, mouseStartPosition.Y);

	//selectedActors.Empty();
	//for (int i = 0; i < selectedUnits.Num(); i++)
	//{
	//	selectedUnits[i]->OnUnitSelected(false);
	//}
	//selectedUnits.Empty();

	if (!unitComponents.Num() == 0) { return; }
	for (int32 i = 0; i < unitComponents.Num(); i++)
	{
		unitComponents[i]->SetIsUnitSelected(false);
	}
	unitComponents.Empty();
}

void AMainHud::StopDraw()
{
	//Stop drawing
	bIsDrawing = false;
}

