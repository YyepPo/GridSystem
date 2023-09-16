#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHud.generated.h"

class UShowBuilding;

UCLASS()
class GRIDSYSTEM_API AMainHud : public AHUD
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	void SetUpBuildingSelectioHUD();
	void SetUpResourceWidget();
	void SetUpShowBuildingWidget();

	void StartDraw();
	void StopDraw();


	FORCEINLINE UShowBuilding* GetShowBuildingWidget() const { return ShowBuildingWidget; }
private:
	void UnitSelectionBehaviour();
	void DrawRectangle();
	void OnUnitSelection();

	APlayerController* playerController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UBuildingSelection> BuildingHUDClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UResourceWidget> ResourceWidgetClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UShowBuilding> ShowBuildingWidgetClass;

	bool bIsDrawing;
	FVector2D mouseStartPosition;
	FVector2D mouseEndPosition;
	UPROPERTY(EditAnywhere,Category = "Rectangle")
		FLinearColor RectangleSelectionColor;
	UPROPERTY(EditAnywhere, Category = "Rectangle")
		TSubclassOf<class AUnitBase> UnitBaseClass;
	UPROPERTY(EditAnywhere, Category = "Rectangle")
		TSubclassOf<class UUnitComponent> unitComponentClass;
	TArray<UUnitComponent*> unitComponents;


	TArray<AActor*> selectedActors;
	TArray<AActor*> qweqwe;
public:
	UPROPERTY()
		UBuildingSelection* BuildingHUD;
	UPROPERTY(BlueprintReadWrite)
		UShowBuilding* ShowBuildingWidget;
	UPROPERTY()
		UResourceWidget* ResourceWidget;

	FORCEINLINE TArray<UUnitComponent*> GetSelectedUnitComponents() const { return unitComponents; }
};
