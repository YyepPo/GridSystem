#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GridManagerInstance.generated.h"

class AGridRepresentative;

UCLASS()
class GRIDSYSTEM_API UGridManagerInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void SpawnGrids();
private:
		UPROPERTY(EditAnywhere,Category = "Grid")
		float Rows;
	UPROPERTY(EditAnywhere,Category = "Grid")
		float Columns;
	UPROPERTY(EditAnywhere,Category = "Grid")
		float CellSize;
	UPROPERTY(EditAnywhere,Category = "Grid")
		float zOffset;
	UPROPERTY(EditAnywhere, Category = "Grid")
		TSubclassOf<AGridRepresentative> gridRepresentativeClass;
};
