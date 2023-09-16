#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingLevelUpWidget.generated.h"

class UBorder;
class UTextBlock;
class UButton;

UCLASS()
class GRIDSYSTEM_API UBuildingLevelUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Level Texts
	void SetCurrentLevelText(int32 currentLevel);
	void SetNextLevelText(int32 nextLevel);
	void SetMaxLevel(int32 maxLevel);
	void DisableUpgradeWidget();
	// Upgrade Texts
	void SetUpgradeCostTexts(int32 peopleAmount, int32 coinAmount, int32 stoneAmount, int32 woodAmount);

	FORCEINLINE UButton* GetTickButton() const { return TickButton; }
private:
	FText ConvertNumberToText(int32 number,FString word);

	UPROPERTY(meta = (BindWidget))
		UBorder* WidgetBorder;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* UpgradeBuildingText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentLevelTextBlock;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentLevelText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* NextLevelTextBlock;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* NextLevelText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MaxLevelTextBlock;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MaxLevelText;

	UPROPERTY(meta = (BindWidget))
		UButton* TickButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CrossButton;

	/// <summary>
	/// Upgrade Cost
	/// </summary>
	UPROPERTY(meta = (BindWidget))
		class UImage* UpgradeCostBackgroundImage;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* UpgradeVerticalBox;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* UpgradeCostText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* People;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Coin;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Stone;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Wood;
};
