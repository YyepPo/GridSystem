#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeathInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDeathInterface : public UInterface
{
	GENERATED_BODY()
};

class GRIDSYSTEM_API IDeathInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual bool OnDeathInterface() = 0;
};
