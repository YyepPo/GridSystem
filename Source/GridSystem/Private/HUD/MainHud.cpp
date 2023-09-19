#include "HUD/MainHud.h"
#include "HUD/BuildingSelection.h"
#include "HUD/ResourceWidget.h"
#include "HUD/ShowBuilding.h"

#include "UnitBase.h"
#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Blueprint/UserWidget.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

void AMainHud::BeginPlay()
{
	Super::BeginPlay();
}

void AMainHud::DrawHUD()
{
	Super::DrawHUD();
	
	UnitSelectionBehaviour();
}

void AMainHud::SetUpBuildingSelectionHUD()
{
	//Add the buildingSelectionWidget to viewport if exists
	playerController = (playerController == nullptr) ? GetOwningPlayerController() : playerController;
	if (!playerController) { return; }

	BuildingHUD = CreateWidget<UBuildingSelection>(playerController, BuildingHUDClass);
	if (BuildingHUD) BuildingHUD->AddToViewport();

}

void AMainHud::SetUpResourceWidget()
{

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
	TArray<AActor*> actors;
	GetActorsInSelectionRectangle(mouseStartPosition, mouseEndPosition, actors, false, true);
	//Loop through all selectedActors
	for (int i = 0; i < actors.Num(); i++)
	{

		UUnitComponent* comp = Cast<UUnitComponent>(actors[i]->GetComponentByClass(unitComponentClass));
		if (comp)
		{
			unitComponents.AddUnique(comp);
			comp->SetIsUnitSelected(true);
		}
	}

}


void AMainHud::StartDraw()
{
	//On startDraw deselect the units that are selected and then start drawing
	bIsDrawing = true;
	GetOwningPlayerController()->GetMousePosition(mouseStartPosition.X, mouseStartPosition.Y);

	for (int32 i = 0; i < unitComponents.Num(); i++)
	{
		unitComponents[i]->SetIsUnitSelected(false);
		UE_LOG(LogTemp, Warning, TEXT("%d"), unitComponents.Num());
	}
	unitComponents.Empty();
}

void AMainHud::StopDraw()
{
	//Stop drawing
	bIsDrawing = false;
}

