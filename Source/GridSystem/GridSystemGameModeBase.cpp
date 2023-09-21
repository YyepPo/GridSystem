#include "GridSystemGameModeBase.h"
#include "Grid/GridManagerInstance.h"

#include "Buildings/MotherBuilding.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AGridSystemGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid();

	//if mother building is destroyed, player loses the game
	if (AMotherBuilding* motherBuilding = Cast<AMotherBuilding>(UGameplayStatics::GetActorOfClass(this, motherBuildingClass)))
			motherBuilding->OnMotherBuildingDestroyedSignature.AddDynamic(this, &AGridSystemGameModeBase::OnMotherBuildingDestroyedHandler);
}

void AGridSystemGameModeBase::SpawnGrid() const
{
	UGridManagerInstance* gridManagerInstance = Cast<UGridManagerInstance>(GetGameInstance());
	if (gridManagerInstance) gridManagerInstance->SpawnGrids();
}

