#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GridPlayerController.generated.h"

UCLASS()
class GRIDSYSTEM_API AGridPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
		class AMainHud* HUD;
	class UShowBuilding* ShowBuildingReference;
	class UResourceWidget* ResourceWidgetReference;
	UPROPERTY(BlueprintReadWrite)
		class UBuildingSelection* BuildingSelectionWidgetReference;

private:

};
