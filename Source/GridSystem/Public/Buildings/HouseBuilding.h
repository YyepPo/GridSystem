#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "HouseBuilding.generated.h"

UCLASS()
class GRIDSYSTEM_API AHouseBuilding : public ABaseBuilding
{
	GENERATED_BODY()
public:
	virtual void BuildingFunctionality();
protected:
	virtual void BuildingFunctionalityTimer() override;
	virtual void LevelUpFunctionality() override;
};
