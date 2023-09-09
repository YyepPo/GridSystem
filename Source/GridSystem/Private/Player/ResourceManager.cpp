#include "Player/ResourceManager.h"

#include "PlayerController/GridPlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "HUD/ResourceWidget.h"


#define PrintLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

UResourceManager::UResourceManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResourceManager::BeginPlay()
{
	Super::BeginPlay();
	
}
