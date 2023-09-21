#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "ArcherGenerator.generated.h"

class AUnitBase;
class ANewFriendlyUnit;
class UUnitsGridComponent;

UCLASS()
class GRIDSYSTEM_API AArcherGenerator : public ABaseBuilding
{
	GENERATED_BODY()

public:
	AArcherGenerator();
	virtual void BuildingFunctionality() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* UnitsGridLocation;
protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	virtual void BuildingFunctionalityTimer() override;
	virtual void LevelUpFunctionality() override;
	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;

private:

	UPROPERTY(EditAnywhere)
		UUnitsGridComponent* UnitsGridComponent;

	UPROPERTY(EditAnywhere, Category = "Units")
		bool bIsFriendlyBuilding = true;

	UPROPERTY(EditAnywhere, Category = "Units")
		TSubclassOf<ANewFriendlyUnit> UnitToBeSpawned;
	UPROPERTY(VisibleAnywhere, Category = "Units")
		int32 spawnedUnitCounter; 
	UPROPERTY(VisibleAnywhere, Category = "Units")
		TArray<ANewFriendlyUnit*> spawnedUnits;
	UPROPERTY(EditAnywhere, Category = "Units")
		TSubclassOf<ANewFriendlyUnit> UnitToBeSpawnedLevel2;

};
