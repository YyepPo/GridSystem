#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitsGridComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UUnitsGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitsGridComponent();
	
	void SpawnUnitsGrid();
protected:
	virtual void BeginPlay() override;
private:

	friend class AArcherGenerator;
	AArcherGenerator* ArcherBuilding;

	UPROPERTY(EditAnywhere, Category = "Grid")
		float Rows;
	UPROPERTY(EditAnywhere, Category = "Grid")
		float Columns;
	UPROPERTY(EditAnywhere, Category = "Grid")
		float CellSize;
	UPROPERTY(EditAnywhere, Category = "Grid")
		TSubclassOf<class AGridRepresentative> GridRepresentativeClass;
public:
	FORCEINLINE void SetComponentOwner(class AArcherGenerator* OwnerArcherGenerator) { ArcherBuilding = OwnerArcherGenerator; }
	UPROPERTY()
		TArray<AGridRepresentative*> grids;
};
