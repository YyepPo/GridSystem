#include "Player/ResourceManager.h"

#define PrintLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

UResourceManager::UResourceManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResourceManager::BeginPlay()
{
	Super::BeginPlay();
	
}
