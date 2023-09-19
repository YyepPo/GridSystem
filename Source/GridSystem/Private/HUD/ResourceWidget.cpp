#include "HUD/ResourceWidget.h"

#include "Components/TextBlock.h"

void UResourceWidget::UpdatePeopleAmountText(int32 newPeopleAmount) const
{
	FString text = FString::FromInt(newPeopleAmount);
	if(PeopleAmountText)PeopleAmountText->SetText(FText::FromString(text));
}

void UResourceWidget::UpdateCoinAmountText(int32 newCoinAmount) const
{
	FString text = FString::FromInt(newCoinAmount);
	if(CoinText)CoinText->SetText(FText::FromString(text));
}

void UResourceWidget::UpdateWoodAmountText(int32 newWoodAmount) const
{
	FString text = FString::FromInt(newWoodAmount);
	if(WoodAmountText)WoodAmountText->SetText(FText::FromString(text));
}

void UResourceWidget::UpdateStoneAmountText(int32 newStoneAmount) const
{
	FString text = FString::FromInt(newStoneAmount);
	if(StoneAmountText)StoneAmountText->SetText(FText::FromString(text));
}