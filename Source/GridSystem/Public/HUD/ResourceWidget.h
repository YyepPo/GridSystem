#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidget.generated.h"

UCLASS()
class GRIDSYSTEM_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdatePeopleAmountText(int32 newPoepleAmount);
	void UpdateCoinAmountText(int32 newCoinAmount);
	void UpdateWoodAmountText(int32 newWoodAmount);
	void UpdateStoneAmountText(int32 newStoneAmount);

	UPROPERTY(meta = (BindWidget))
		class UBorder* BackgroundBorder;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PeopleAmountText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CoinText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* WoodAmountText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StoneAmountText;

	UPROPERTY(meta = (BindWidget))
		class UImage* PeopleImage;
	UPROPERTY(meta = (BindWidget))
		UImage* CoinImage;
	UPROPERTY(meta = (BindWidget))
		UImage* WoodImage;
	UPROPERTY(meta = (BindWidget))
		UImage* StoneImage;
};
