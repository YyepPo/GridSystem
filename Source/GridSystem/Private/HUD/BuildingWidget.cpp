#include "HUD/BuildingWidget.h"
#include "HUD/ShowBuilding.h"
#include "HUD/BuildingSelection.h"

#include "Player/PlayerCharacter.h"
#include "PlayerController/GridPlayerController.h"

#include "KISMET/Gameplaystatics.h"

#include "Components/Image.h"
#include "Components/Button.h"

void UBuildingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Get playerCharacter class
	Player = (Player == nullptr) ? Cast<APlayerCharacter>(GetOwningPlayerPawn()) : Player;

	//Get playerController
	PlayerController = Cast<AGridPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	Button->OnClicked.AddDynamic(this, &UBuildingWidget::ButtonClickedEvent);
}

void UBuildingWidget::ButtonClickedEvent()
{
	if (PlayerController)
		BuildingSelection = (BuildingSelection == nullptr) ? BuildingSelection = PlayerController->BuildingSelectionWidgetReference : BuildingSelection;

	//On this item clicked update the ShowBuilding's widget like building name ,description etc.
	if (BuildingSelection && BuildingSelection->ShowSelectedBuildingWidgetClass)
	{
		BuildingSelection->ShowSelectedBuildingWidgetClass->SetVisibility(ESlateVisibility::Visible);
		BuildingSelection->ShowSelectedBuildingWidgetClass->SetAllWidgets(ThumbnailRef,
			BuildingName,
			BuildingDescription,
			BuildingResources.BuildingPrice,
			BuildingResources.RequiredWoodAmount,
			BuildingResources.RequiredStoneAmount,
			this);
	}

}

void UBuildingWidget::OnWidgetSelected()
{

}
