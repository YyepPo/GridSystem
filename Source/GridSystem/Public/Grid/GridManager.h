#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class GRIDSYSTEM_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridManager();
protected:
	virtual void BeginPlay() override;

private:	

};
