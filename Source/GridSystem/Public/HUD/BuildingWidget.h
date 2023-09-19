#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingWidget.generated.h"


USTRUCT(BlueprintType)
struct FBuildingResources
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int32 BuildingPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RequiredWoodAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 RequiredStoneAmount;
};

UCLASS()
class GRIDSYSTEM_API UBuildingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void OnWidgetSelected();

	UFUNCTION()
		virtual void ButtonClickedEvent();

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UButton* Button;
	UPROPERTY(meta = (BindWidget))
		class UImage* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ThumbnailRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString BuildingName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString BuildingDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBuildingResources BuildingResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseBuilding> BuildingClass;
protected:

	UPROPERTY()
		class APlayerCharacter* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UShowBuilding> ShowBuildingWidgetClass;
	UPROPERTY()
		UShowBuilding* ShowBuilding;

	UPROPERTY()
		class AGridPlayerController* PlayerController;
	UPROPERTY()
	class UBuildingSelection* BuildingSelection;
};
