#include "HUD/BuildingLevelUpWidget.h"

#include "Components/TextBlock.h"

#pragma region Levels

void UBuildingLevelUpWidget::SetCurrentLevelText(int32 currentLevel)
{
	CurrentLevelText->SetText(ConvertNumberToText(currentLevel,""));
}

void UBuildingLevelUpWidget::SetNextLevelText(int32 nextLevel)
{
	NextLevelText->SetText(ConvertNumberToText(nextLevel,""));
}

void UBuildingLevelUpWidget::SetMaxLevel(int32 maxLevel)
{
	MaxLevelText->SetText(ConvertNumberToText(maxLevel,""));
}

#pragma endregion LevelTexts

#pragma region UpgradeTexts

void UBuildingLevelUpWidget::SetUpgradeCostTexts(int32 peopleAmount,int32 coinAmount,int32 stoneAmount,int32 woodAmount)
{
	People->SetText(ConvertNumberToText(peopleAmount,"People: "));
	Coin->SetText(ConvertNumberToText(coinAmount, "Coin: "));
	Stone->SetText(ConvertNumberToText(stoneAmount, "Stone: "));
	Wood->SetText(ConvertNumberToText(woodAmount, "Wood: "));
}

#pragma endregion UpgradeTexts

FText UBuildingLevelUpWidget::ConvertNumberToText(int32 number,FString word)
{
	FString string = word + FString::FromInt(number);
	FText text = FText::FromString(string);
	return text;
}