#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsAttackableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIsAttackableInterface : public UInterface
{
	GENERATED_BODY()
};

class GRIDSYSTEM_API IIsAttackableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual void IsAttackAble() = 0;
};
