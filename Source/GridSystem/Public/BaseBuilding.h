#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuilding.generated.h"

class UWidget;
class UWidgetComponent;
class UBoxComponent;
class UBuildingsLevelUpComponent;
class UBuildingLevelUpWidget;
class APlayerCharacter;
class UResourceManager;
class UBuildingDataAsset;

UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	EBS_NotPlaced UMETA(DisplayName = "Not Placed"),
	EBS_Placed UMETA(DisplayName = "Placed"),
	EGS_NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	EBT_House UMETA(DisplayName = "House"),
	EBT_CoinGenerator UMETA(DisplayName = "Coin Generator"),
	EBT_WoodGenerator UMETA(DisplayName = "Wood Generator"),
	EBT_StoneGenerator UMETA(DisplayName = "Stone Generator"),

	EBT_NONE UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FBuildingLevelData
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
		int32 currentLevel = 0;
	UPROPERTY(EditAnywhere)
		int32 maxLevel;
};

USTRUCT(BlueprintType)
struct FUpgradeCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int32 peopleAmount;
	UPROPERTY(EditAnywhere)
		int32 coinAmount;
	UPROPERTY(EditAnywhere)
		int32 stoneAmount;
	UPROPERTY(EditAnywhere)
		int32 woodAmount;
};

UCLASS()
class GRIDSYSTEM_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseBuilding();
	//Building functionality when placed
	virtual void BuildingFunctionality();

	UPROPERTY(BlueprintReadWrite)
		UWidget* LevelUpWidgetComponent;
	UPROPERTY()
		UBuildingLevelUpWidget* BuildingLevelUpWidget;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UPROPERTY()
		APlayerCharacter* PlayerCharacter;
	UPROPERTY()
		UResourceManager* PlayerResourceManager;

	void LevelBasedResourceGain();
	UFUNCTION(BlueprintImplementableEvent)
		void PopUpResourceWidget();
	UPROPERTY(EditAnywhere,Category = "Resource")
		float resourceAmount;
	UPROPERTY(EditAnywhere,Category = "Resource")
		TArray<float> resourceAmounts;

	/// <summary>
	/// Building functionality
	/// </summary>
	virtual void BuildingFunctionalityTimer();
	UFUNCTION(BlueprintImplementableEvent)
		void OccupyNeighbours();
	FTimerHandle BuildingFunctionalityTimerHandle;
	UPROPERTY(EditAnywhere,Category = "Grid")
		bool bOccupiesNeighbours;
	UPROPERTY(EditAnywhere,Category = "Building")
		TSubclassOf<class UCameraShakeBase> buildingPlacedCameraShake;
	UPROPERTY(EditAnywhere, Category = "Building")
		float functionTimeRate;
	/// <summary>
	/// Level 
	/// </summary>
	UFUNCTION()
		void TickButtonClicked();
	virtual void LevelUpFunctionality();
	bool HasPlayerEnoughResourcesToLevelUpBuilding();
	bool HasEnoughResource(int32 index);
	void ConsumeMaterials(int32 index);
	bool HasReachedMaxLevel() const;
	void SetUpLevelWidgetTexts(int32 currentLevel) const;
	void SetUpUpgradeCostText();

	UPROPERTY(EditAnywhere,Category = "Level")
		FBuildingLevelData buildingLevelData;
	UPROPERTY(EditAnywhere, Category = "Level")
		TArray<FUpgradeCost> upgradeCosts;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh;

	/// <summary>
	/// Resources
	/// </summary>
	UPROPERTY(EditAnywhere,Category = "Resources")
		TSubclassOf<class AResource> resourceClass;
	UPROPERTY()
		AResource* resource;

	//Data Asset
	UPROPERTY(EditAnywhere, Category = "MyData")
		TSoftObjectPtr<UBuildingDataAsset> buildingDataAsset;
	UPROPERTY()
		UBuildingDataAsset* loadedBuildingDataAsset;
private:
	UFUNCTION(BlueprintCallable,meta = (AllowPrivateAccess = "true"))
		void SetResource(AResource* newResource) {resource = newResource;}

	void SetUpLevelWidget();
	void BindingMethodsToDelegates();

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseBuilding> ClassRepresentative;
	bool bIsClickable = true;

	//Enums
	UPROPERTY(VisibleAnywhere)
		EBuildingState BuildingState = EBuildingState::EBS_NotPlaced;
	UPROPERTY(VisibleAnywhere)
		EBuildingType BuildingType = EBuildingType::EBT_NONE;

	//Components
	UPROPERTY(EditAnywhere,meta = (AllowPrivateAccess = "true"),BlueprintReadWrite	)
		UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UBuildingsLevelUpComponent* BuildingLevelManager;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* ResourcePopUpWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* buildingLevelUpWidgetComponent;

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetLevelUpWidget(UWidget* NewWidget) { LevelUpWidgetComponent = NewWidget; }
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FORCEINLINE float GetResourceAmount() const { return resourceAmount; }
	FORCEINLINE TSubclassOf<ABaseBuilding> GetClassRepresentative() const { return ClassRepresentative; }
	FORCEINLINE void SetBuildingState(EBuildingState NewBuildingState) { BuildingState = NewBuildingState; }
	FORCEINLINE EBuildingType GetBuildingType() const { return BuildingType; }
	FORCEINLINE bool GetOccupiesNeighbours() const { return bOccupiesNeighbours; }
};
