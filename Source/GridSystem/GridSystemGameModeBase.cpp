#include "GridSystemGameModeBase.h"
#include "Grid/GridManagerInstance.h"

void AGridSystemGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGridManagerInstance* gridManagerInstance = Cast<UGridManagerInstance>(GetGameInstance());
	if (gridManagerInstance) gridManagerInstance->SpawnGrids();
}
