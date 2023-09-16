#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowBuilding.generated.h"

class UImage;
class UBuildingWidget;
class UTextBlock;
class UButton;
class UBorder;

UCLASS()
class GRIDSYSTEM_API UShowBuilding : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void EnableShowBuildingWidgets(bool bIsVisible);
	void SetAllWidgets(UTexture2D* NewThumbnail, FString newBuildingName, FString newBuildingDescription, int32 newPrice,int32 newWoodAmount,int32 newStoneAmount,UBuildingWidget* NewBuildingWidget);
	UFUNCTION(BlueprintCallable)
		void ChangePurchaseButtonVisibilityBasedOnResources();
	UFUNCTION(BlueprintImplementableEvent)
		void OnBuildingPurchase();

	UPROPERTY(meta = (BindWidget))
		UBorder* BackgroundBorder;
	UPROPERTY(meta = (BindWidget))
		UBorder* BuildingThumbnailBorder;
	UPROPERTY(meta = (BindWidget))
		UImage* BackgroundImage;
	UPROPERTY(meta = (BindWidget))
		UImage* BuildingThumbnail;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* BuildingName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PriceText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BuildingDescription;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BuildingPrice;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* WoodPrice;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StonePrice;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DescriptionText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CoinText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* WoodText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StoneText;

	UPROPERTY(meta = (BindWidget))
		UButton* BuyButton;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BuyText;

	UFUNCTION(BlueprintCallable)
		void SetResource(class AResource* newResource) {resource = newResource;}
private:
	UFUNCTION()
		void BuyButtonEvent();

	bool CanPurchase();
	void DisabePurchaseButton(bool bIsDisabled);
	void PurchaseBuilding();
	void ConsumeResources();

	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere,Category = "Resources")
		TSubclassOf<class AResource> resourceClass;
	UPROPERTY()
		AResource* resource;

	UBuildingWidget* BuildingWidget;

	//when a uwidget component is disabled
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FLinearColor disabledColor;
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* purchaseSound;
};
