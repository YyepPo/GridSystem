#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "WoodGeneratorBuilding.generated.h"

UCLASS()
class GRIDSYSTEM_API AWoodGeneratorBuilding : public ABaseBuilding
{
	GENERATED_BODY()
public:
	virtual void BuildingFunctionality() override;
protected:
	virtual void BuildingFunctionalityTimer() override;
	virtual void OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed) override;
	virtual void LevelUpFunctionality() override;
};
