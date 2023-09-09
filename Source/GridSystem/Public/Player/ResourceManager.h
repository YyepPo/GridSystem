#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UResourceManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceManager();
protected:
	virtual void BeginPlay() override;

private:	

public:

};
