#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "StoneGenerator.generated.h"

UCLASS()
class GRIDSYSTEM_API AStoneGenerator : public ABaseBuilding
{
	GENERATED_BODY()
public:
	virtual void BuildingFunctionality() override;
protected:
	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;
	virtual void BuildingFunctionalityTimer() override;
	virtual void LevelUpFunctionality() override;
private:
};
