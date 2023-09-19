#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingsLevelUpComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UBuildingsLevelUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildingsLevelUpComponent();

protected:
	virtual void BeginPlay() override;
};
