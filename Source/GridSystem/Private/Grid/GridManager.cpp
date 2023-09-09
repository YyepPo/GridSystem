#include "Grid/GridManager.h"

#include "Grid/GridRepresentative.h"

AGridManager::AGridManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();

}
