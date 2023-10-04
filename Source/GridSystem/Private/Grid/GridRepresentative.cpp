#include "Grid/GridRepresentative.h"
#include "Components/BoxComponent.h"

AGridRepresentative::AGridRepresentative() :
	boxCollider{ CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider"))) },
	staticMesh{ CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh"))) }
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(boxCollider);
	staticMesh->SetupAttachment(GetRootComponent());
}

void AGridRepresentative::BeginPlay()
{
	Super::BeginPlay();
}

void AGridRepresentative::OccupyGrid()
{
	//Check if the grid is already occupied
	if (GridState == EGridState::EGS_Occupied) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("%s is occupied. Cannot add any objects"), *GetActorNameOrLabel());
		return; 
	}

	//Change the GrisState to occupied
	GridState = EGridState::EGS_Occupied;

	ChangeMaterialColor(occupiedColor, occupiedColor);
}

void AGridRepresentative::UnOccupyGrid()
{
	//Check if grid is occupied
	if (GridState != EGridState::EGS_Occupied) { return; }

	//Update grid state to unoccupy , so another building can be attached to this grid
	GridState = EGridState::EGS_UnOccupied;

	ChangeMaterialColor(occupiedColor,unOccupiedColor);
}

void AGridRepresentative::ChangeMaterialColor(FLinearColor newOccupiedColor,FLinearColor newUnoccupiedColor) const
{
	UMaterialInstanceDynamic* materialInstance = staticMesh->CreateDynamicMaterialInstance(0);
	if (materialInstance)
	{
		const FName baseColorParameter = FName(TEXT("BaseColor"));
		const FName secondColorParameter = FName(TEXT("SecondColor"));
		materialInstance->SetVectorParameterValue(baseColorParameter, newOccupiedColor);
		materialInstance->SetVectorParameterValue(secondColorParameter, newUnoccupiedColor);
	}
}

