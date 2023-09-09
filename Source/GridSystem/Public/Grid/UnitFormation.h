#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitFormation.generated.h"

class AGridRepresentative;

UCLASS()
class GRIDSYSTEM_API AUnitFormation : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnitFormation();

protected:
	virtual void BeginPlay() override;

private:

};
