#include "Grid/GridRepresentative.h"
#include "Components/BoxComponent.h"

AGridRepresentative::AGridRepresentative()
{
	PrimaryActorTick.bCanEverTick = false;

	InitializeComponents();
}

void AGridRepresentative::InitializeComponents()
{
	//Create a BoxComponent(BoxCollider) to handle collision
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider")));
	SetRootComponent(BoxCollider);

	//Create a StaticMeshComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh")));
	StaticMesh->SetupAttachment(GetRootComponent());
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

	//When occupied change the grid's material color to (OccupiedColor)
	//so the players knows if the grid is occupied or not
	UMaterialInstanceDynamic* materialInstance = StaticMesh->CreateDynamicMaterialInstance(0);
	if (materialInstance)
	{
		const FName baseColorParameter = FName(TEXT("BaseColor"));
		materialInstance->SetVectorParameterValue(baseColorParameter, OccupiedColor);
	}
}

