
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingSelection.generated.h"

UCLASS()
class GRIDSYSTEM_API UBuildingSelection : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BuildingSelectionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
		class UButton* BackGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* VerticalBoxBuildings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* BuildingWidgetScroll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UBuildingWidget* Slot1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBuildingWidget* Slot2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBuildingWidget* Slot3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBuildingWidget* Slot4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBuildingWidget* Slot5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UShowBuilding* ShowSelectedBuildingWidgetClass;
};
