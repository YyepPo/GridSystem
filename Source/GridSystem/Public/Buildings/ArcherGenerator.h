#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "ArcherGenerator.generated.h"

class AUnitBase;
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
	virtual void LevelupFunctionality() override;
	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;

private:

	UPROPERTY(EditAnywhere)
		UUnitsGridComponent* UnitsGridComponent;

	UPROPERTY(EditAnywhere, Category = "Units")
		bool bIsFriendlyBuilding = true;

	UPROPERTY(EditAnywhere, Category = "Units")
		TSubclassOf<AUnitBase> UnitToBeSpawned;
	UPROPERTY(EditAnywhere, Category = "Units")
		int32 spawnedUnitCounter;
	UPROPERTY(VisibleAnywhere, Category = "Units")
		TArray<AUnitBase*> spawnedUnits;
	UPROPERTY(EditAnywhere, Category = "Units")
		TSubclassOf<AUnitBase> UnitToBeSpawnedLevel2;

};
