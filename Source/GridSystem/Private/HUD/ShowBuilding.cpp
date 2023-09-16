#include "HUD/ShowBuilding.h"
#include "HUD/BuildingWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Player/PlayerCharacter.h"
#include "Player/ResourceManager.h"
#include "Resource/Resource.h"

#include "Kismet/GameplayStatics.h"

void UShowBuilding::NativeConstruct()
{
	Super::NativeConstruct();
	//Show the selectedBuilding widgets on screen
	EnableShowBuildingWidgets(true);
	
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	BuyButton->OnClicked.AddDynamic(this, &UShowBuilding::BuyButtonEvent);
}

void UShowBuilding::EnableShowBuildingWidgets(bool bIsVisible)
{
	//Change the widgets visibility
	ESlateVisibility visibility;

	(bIsVisible == true) ? visibility = ESlateVisibility::Visible : visibility = ESlateVisibility::Hidden;

	BackgroundImage->SetVisibility(visibility);
	BuildingThumbnail->SetVisibility(visibility);
	BuildingName->SetVisibility(visibility);
	BuildingDescription->SetVisibility(visibility);
	BuildingPrice->SetVisibility(visibility);
	
}

void UShowBuilding::SetAllWidgets(UTexture2D* NewThumbnail, FString newBuildingName, FString newBuildingDescription, int32 newPrice, int32 newWoodAmount, int32 newStoneAmount, UBuildingWidget* NewBuildingWidget)
{
	//Update all texts and textures based on given parameter

	BuildingThumbnail->SetBrushFromTexture(NewThumbnail);
	BuildingName->SetText(FText::FromString(newBuildingName));
	BuildingDescription->SetText(FText::FromString(newBuildingDescription));

	FString priceText = FString::FromInt(newPrice);
	BuildingPrice->SetText(FText::FromString(priceText));

	FString woodPriceText = FString::FromInt(newWoodAmount);
	WoodPrice->SetText(FText::FromString(woodPriceText));

	FString stonePriceText = FString::FromInt(newStoneAmount);
	StonePrice->SetText(FText::FromString(stonePriceText));

	BuildingWidget = NewBuildingWidget;

	CanPurchase();
}

void UShowBuilding::BuyButtonEvent()
{
	//If there is not enough materials player cannot purchase the building
	if (!CanPurchase()) {
		return; }
	//If player has enough materials then the building can be purchased
	PurchaseBuilding();
}

bool UShowBuilding::CanPurchase()
{
	//Check if the player has enough materials to purchase a building

	if (!resource) { 
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, FString::Printf(TEXT("There is no ResourceManagerComponent in can purchase method")));
		return false; }

	const bool bNotEnoughCoin = resource->GetCoinAmount() < BuildingWidget->BuildingResources.BuildingPrice;
	const bool bNotEnoughWood = resource->GetWoodAmount() < BuildingWidget->BuildingResources.RequiredWoodAmount;
	const bool bNotEnoughStone = resource->GetStoneAmount() < BuildingWidget->BuildingResources.RequiredStoneAmount;

	if (bNotEnoughCoin && bNotEnoughWood && bNotEnoughStone)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("You dont have enough coin")));
		//If player doesn't have the required materials then inform the player by changing purchase button's opacity
		DisabePurchaseButton(true);
		return false;
	}

	return true;
}

void UShowBuilding::DisabePurchaseButton(bool bIsDisabled)
{
	//Change the purchase button's opacity;
	FLinearColor textColor = BuyText->ColorAndOpacity.GetSpecifiedColor();
	(bIsDisabled) ? textColor.A = .5f : textColor.A = 1.f;
	FSlateColor newOpacity = textColor;
	BuyText->SetColorAndOpacity(newOpacity);
}

void UShowBuilding::PurchaseBuilding()
{
	if (!BuildingWidget && !resource) {return; }

	const int32 coinAmount = resource->GetCoinAmount();
	const int32 woodAmount = resource->GetWoodAmount();
	const int32 stoneAmount = resource->GetStoneAmount();
	//check if there are enough resource to purchase a building
	if (coinAmount <= 0 || woodAmount <= 0 || stoneAmount <= 0)
	{
		DisabePurchaseButton(true);

		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Blue, FString::Printf(TEXT("Not enough resources")));
		return;
	}

	DisabePurchaseButton(false);

	//On Purchase hide the widget(Play out anim from WBP BuildingSelection).This function is a BlueprintImplementableEvent
	OnBuildingPurchase();

	//if player has enough materials then consume resources
	ConsumeResources();
	//Set the players currentBuilding to the selected building class
	if(PlayerCharacter) PlayerCharacter->SelectBuilding(BuildingWidget->BuildingClass);
	//Play purchase sound
	if (purchaseSound) UGameplayStatics::PlaySound2D(this,purchaseSound);
}

void UShowBuilding::ConsumeResources()
{
	//Consume all the necessary resources
	resource->RemoveCoin(BuildingWidget->BuildingResources.BuildingPrice);
	resource->RemoveWood(BuildingWidget->BuildingResources.RequiredWoodAmount);
	resource->RemoveStone(BuildingWidget->BuildingResources.RequiredStoneAmount);
}

void UShowBuilding::ChangePurchaseButtonVisibilityBasedOnResources()
{
	CanPurchase() ? DisabePurchaseButton(true) : DisabePurchaseButton(false);
}