#include "Resource/Resource.h"
#include "HUD/ResourceWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/GridPlayerController.h"
AResource::AResource()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AResource::BeginPlay()
{
	Super::BeginPlay();
	
	AGridPlayerController* playerController = Cast<AGridPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (playerController && resourceWidgetClass)
	{
		resourceWidget = CreateWidget<UResourceWidget>(playerController, resourceWidgetClass);
		if (resourceWidget)
		{
			resourceWidget->AddToViewport();
			resourceWidget->UpdatePeopleAmountText(resources.currentPeopleAmount);
			resourceWidget->UpdateCoinAmountText(resources.currentCoinAmount);
			resourceWidget->UpdateWoodAmountText(resources.currentWoodAmount);
			resourceWidget->UpdateStoneAmountText(resources.currentStoneAmount);
		}
	}

}

void AResource::AddPeople(int peopleAmount)
{
	//Add people to currentPeopleAmount and then update peopleAmountText
	resources.currentPeopleAmount += peopleAmount;
	if (resourceWidget)
		resourceWidget->UpdatePeopleAmountText(resources.currentPeopleAmount);

}
void AResource::RemovePeople(int peopleAmount)
{
	//Remove people from currentPeopleAmount and then update peopleAmountText
	resources.currentPeopleAmount -= peopleAmount;
	if (resourceWidget) resourceWidget->UpdatePeopleAmountText(resources.currentPeopleAmount);
}

void AResource::AddCoin(int coinAmount)
{
	//Add coin to currentCoinAmount and then update coinAmountText
	resources.currentCoinAmount += coinAmount;
	if (resourceWidget) resourceWidget->UpdateCoinAmountText(resources.currentCoinAmount);
}
void AResource::RemoveCoin(int coinAmount)
{
	//Remove coin from currentCoinAmount and then update coinAmountText
	resources.currentCoinAmount -= coinAmount;
	if (resourceWidget) resourceWidget->UpdateCoinAmountText(resources.currentCoinAmount);
}

void AResource::AddWood(int woodAmount)
{
	//Add wood to currentWoodAmount and then update woodAmountText
	resources.currentWoodAmount += woodAmount;
	if (resourceWidget) resourceWidget->UpdateWoodAmountText(resources.currentWoodAmount);
}
void AResource::RemoveWood(int woodAmount)
{
	//Remove wood from currentWoodAmount and then update woodAmountText
	resources.currentWoodAmount -= woodAmount;
	if (resourceWidget) resourceWidget->UpdateWoodAmountText(resources.currentWoodAmount);
}

void AResource::AddStone(int stoneAmount)
{
	//Add stone to currentStoneAmount and then update stoneAmountText
	resources.currentStoneAmount += stoneAmount;
	if (resourceWidget)
	{
		resourceWidget->UpdateStoneAmountText(resources.currentStoneAmount);
		UE_LOG(LogTemp, Warning, TEXT("add stone"));
	}
}
void AResource::RemoveStone(int stoneAmount)
{
	//Remove stone from currentStoneAmount and then update stoneAmountText
	resources.currentStoneAmount -= stoneAmount;
	if (resourceWidget) resourceWidget->UpdateStoneAmountText(resources.currentStoneAmount);
}

