#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ABaseBuilding;
class AGridRepresentative;

UCLASS()
class GRIDSYSTEM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SelectBuilding(TSubclassOf<ABaseBuilding> NewBuilding);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class UResourceManager* ResourceManager;// Resource Manager Component
protected:
	virtual void BeginPlay() override;

private:
	void GetTheMainHUD();

	void UpdateCameraLocation();
	void DedectGrid();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AUnitBase> UnitClllas;
	UPROPERTY(EditAnywhere,Category = "Camera Movement")
		float cameraEdgeOffset;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
		float cameraMoveSpeed;

#pragma region InputBindings

	void AttachBuildingToGrid();

	//void ShowWidget();

	void UnitBehaviour();
#pragma endregion InputBindings

#pragma region MainComponents

	//Components
	void InitializeComponents();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

#pragma endregion MainComponents
	
	void LeftMouseButtonPressed();
	void LeftMouseButtonReleased();
	bool bIsHoldingLeftMouse;
	UPROPERTY()
		class AMainHud* MainHud;

	UPROPERTY()
		class AGridPlayerController* GridPlayerController;
	UPROPERTY(EditAnywhere, Category = "Grid")
		float TraceLength;
	UPROPERTY(EditAnywhere, Category = "Grid")
		TSubclassOf<ABaseBuilding> BuildingClass;

	UPROPERTY()
		AGridRepresentative* currentGrid;
	UPROPERTY()
		ABaseBuilding* currentBuilding;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
		bool bIsInventoryOpen = false;

	bool bNorthNeighbourIsOccupied = false;
	bool bSouthNeighbourIsOccupied = false;
	bool bEastNeighbourIsOccupied = false;
	bool bWesthNeighbourIsOccupied = false;


#pragma region UnitFormation
	UPROPERTY(EditAnywhere,Category = "Grid Formation")
		TSubclassOf<AGridRepresentative> gridRepresentativeClass;
	UPROPERTY(EditAnywhere,Category = "Grid Formation")
		float cellSize;
	UPROPERTY(EditAnywhere,Category = "Grid Formation")
		float columns;
	UPROPERTY(EditAnywhere,Category = "Grid Formation")
		float rows;
	UPROPERTY(VisibleAnywhere,Category = "Grid Formation")
		TArray<AGridRepresentative*> horizontalFormationGrids;

	float rightOffset = 0;
	bool bWasSelected = false;
	FVector formationDirection;
public:
	UFUNCTION(BlueprintCallable)
		void SetFormationToVerticalFormation();
	UFUNCTION(BlueprintCallable)
		void SetFormationToHorizontalFormation();
#pragma endregion
};
