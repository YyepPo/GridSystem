#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MoveToTargetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMoveToTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class GRIDSYSTEM_API IMoveToTargetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void MoveToTargetInterface(AActor* target) = 0;
};
