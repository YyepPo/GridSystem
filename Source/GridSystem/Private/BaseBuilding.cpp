#include "BaseBuilding.h"

#include "Buildings/BuildingsLevelUpComponent.h"

#include "DrawDebugHelpers.h"

#include "Player/PlayerCharacter.h"
#include "Player/ResourceManager.h"//not in use
#include "Resource/Resource.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerController/GridPlayerController.h"

#include "HUD/ShowBuilding.h"

#include "Grid/GridRepresentative.h"

#include "Components/Widget.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "HUD/BuildingLevelUpWidget.h"

#include "Camera/CameraShakeBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ABaseBuilding::ABaseBuilding() :
	ResourcePopUpWidget{ CreateDefaultSubobject<UWidgetComponent>(TEXT("Resource Pop Up Widget")) }
{
	PrimaryActorTick.bCanEverTick = false;
	//Create a Static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh")));
	SetRootComponent(StaticMesh);
	//Create a Box Collider
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider")));
	BoxCollider->SetupAttachment(GetRootComponent());
	//Disable collision so the building can be placed;
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//create buildingsLevelUpComponent
	BuildingLevelManager = CreateDefaultSubobject<UBuildingsLevelUpComponent>(FName(TEXT("Building Level Manager")));

	ResourcePopUpWidget->SetupAttachment(GetRootComponent());
}

void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();

	//Get APlayerCharcater class and from there get player's resource manager
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter) PlayerResourceManager = PlayerCharacter->ResourceManager;

	SetUpLevelWidget();
	
	BindingMethodsToDelegates();

	//Set up level up widget texts
	SetUpLevelWidgetTexts(buildingLevelData.currentLevel);
	SetUpUpgradeCostText();
}

void ABaseBuilding::SetUpLevelWidget()
{
	if (LevelUpWidgetComponent)
	{
		BuildingLevelUpWidget = Cast<UBuildingLevelUpWidget>(LevelUpWidgetComponent);
		LevelUpWidgetComponent->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABaseBuilding::BindingMethodsToDelegates()
{
	if (BoxCollider) BoxCollider->OnClicked.AddDynamic(this, &ABaseBuilding::OnBoxColliderClicked);
	if (BuildingLevelUpWidget) BuildingLevelUpWidget->GetTickButton()->OnClicked.AddDynamic(this, &ABaseBuilding::TickButtonClicked);
}
 
#pragma region LevelUpFunctionality

void ABaseBuilding::OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor is beign clicked"));

	if (LevelUpWidgetComponent) LevelUpWidgetComponent->SetVisibility(ESlateVisibility::Visible);
	
	//Update level texts(current level, next level)
	SetUpLevelWidgetTexts(buildingLevelData.currentLevel);
	//Update upgrade cost texts
	SetUpUpgradeCostText();
}

//when level up tick button is clicked
void ABaseBuilding::TickButtonClicked()
{
	LevelupFunctionality();
}

void ABaseBuilding::LevelupFunctionality()
{
	if (HasReachedMaxLevel()) { return; }

	SetUpLevelWidgetTexts(buildingLevelData.currentLevel);
	SetUpUpgradeCostText();

	//check if player has enough resources
	if (!HasPlayerEnoughResourcesToLevelUpBuilding())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player doesnt have enough resources to upgrade the building"));
		return;
	}

	//increase current level
	buildingLevelData.currentLevel++;

	if (levelUpSound) UGameplayStatics::PlaySoundAtLocation(this, levelUpSound, GetActorLocation());
}

bool ABaseBuilding::HasPlayerEnoughResourcesToLevelUpBuilding()
{
	//check if player has enough resources to upgrade the building
	if (!resource) { return false; }

	for (int32 i = 0; i < upgradeCosts.Num(); i++)
	{
		if (i == buildingLevelData.currentLevel)
		{
			if (HasEnoughResource(i))
			{
				ConsumeMaterials(i);
				return true;
			}
			return false;
		}
	}
	return false;
}

bool ABaseBuilding::HasEnoughResource(int32 index)
{
	const bool bHasEnoughPoeple = resource->GetPoepleAmount() > upgradeCosts[index].peopleAmount;
	const bool bHasEnoughCoin = resource->GetCoinAmount() > upgradeCosts[index].coinAmount;
	const bool bHasEnoughStone = resource->GetStoneAmount() > upgradeCosts[index].stoneAmount;
	const bool bHasEnoughWood = resource->GetWoodAmount() > upgradeCosts[index].woodAmount;
	if (bHasEnoughPoeple && bHasEnoughCoin && bHasEnoughStone && bHasEnoughWood) { return true; }
	return false;
}

void ABaseBuilding::ConsumeMaterials(int32 index)
{
	resource->RemovePeople(upgradeCosts[index].peopleAmount);
	resource->RemoveCoin(upgradeCosts[index].coinAmount);
	resource->RemoveStone(upgradeCosts[index].stoneAmount);
	resource->RemoveWood(upgradeCosts[index].woodAmount);
	UE_LOG(LogTemp, Warning, TEXT("Consume materials"));
}

void ABaseBuilding::LevelBasedResourceGain()
{
	for (int32 i = 0; i < buildingLevelData.maxLevel + 1; i++)
	{
		if (buildingLevelData.currentLevel == i)
		{
			resourceAmount += resourceAmounts[i];
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("%f"), resourceAmount));
			break;
		}
	}
}

bool ABaseBuilding::HasReachedMaxLevel()
{
	if (!BuildingLevelUpWidget) { return false; }

	if (buildingLevelData.currentLevel == buildingLevelData.maxLevel)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, FString::Printf(TEXT("%s has reached max level"),*GetActorNameOrLabel()));
		return true;
	}
	return false;
}

void ABaseBuilding::SetUpLevelWidgetTexts(int32 currentLevel)
{
	if (!BuildingLevelUpWidget) {return;}

	BuildingLevelUpWidget->SetCurrentLevelText(currentLevel);
	BuildingLevelUpWidget->SetNextLevelText(currentLevel);
	BuildingLevelUpWidget->SetMaxLevel(buildingLevelData.maxLevel);
}

void ABaseBuilding::SetUpUpgradeCostText()
{
	if (!BuildingLevelUpWidget) { 
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, FString::Printf(TEXT("no building level up widget")));
		return; }
	for (int32 i = 0; i < upgradeCosts.Num(); i++)
	{
		if (i == buildingLevelData.currentLevel)
		{
			BuildingLevelUpWidget->SetUpgradeCostTexts(upgradeCosts[i].peopleAmount,
				upgradeCosts[i].coinAmount,
				upgradeCosts[i].stoneAmount,
				upgradeCosts[i].woodAmount);
		}
	}
}

#pragma endregion LevelUpFunctionality

#pragma region BuildingFunctionality

void ABaseBuilding::BuildingFunctionality()  
{
	BuildingState = EBuildingState::EBS_Placed;

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
		playerController->ClientStartCameraShake(buildingPlacedCameraShake);

	if (buildingPlacedVFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, buildingPlacedVFX, GetActorLocation());
	if (placedSound) UGameplayStatics::PlaySoundAtLocation(this, placedSound, GetActorLocation());

	//enabled collision after placed , if its not enabled the building cannot be placed because boxCollision blocks the visibility channel
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (GetOccupiesNeighbours()) OccupyNeighbours();

	GetWorldTimerManager().SetTimer(BuildingFunctionalityTimerHandle,this,&ABaseBuilding::BuildingFunctionalityTimer, functionTimeRate,true);
}

void ABaseBuilding::BuildingFunctionalityTimer()
{
	//Building functionality like adding coin or something else
}

#pragma endregion BuildingFunctionality

