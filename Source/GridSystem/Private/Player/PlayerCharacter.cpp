#include "Player/PlayerCharacter.h"
#include "Player/ResourceManager.h"

#include "PlayerController/GridPlayerController.h"

#include "UnitBase.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Grid/GridRepresentative.h"

#include "BaseBuilding.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "HUD/MainHud.h"

#include "NewUnit/UnitComponent.h"



APlayerCharacter::APlayerCharacter() :
	SpringArm{ CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("Spring Arm"))) },
	Camera{ CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera"))) }
{
	PrimaryActorTick.bCanEverTick = true;
	ResourceManager = CreateDefaultSubobject<UResourceManager>(FName(TEXT("Resource Managerr")));
	
	InitializeComponents();
}

void APlayerCharacter::InitializeComponents() const
{
	//Create the spring arm component
	SpringArm->SetupAttachment(GetRootComponent());
	//Create  the camera component and attach it to SpringArm
	Camera->SetupAttachment(SpringArm);

	//Create the CreateTheResourceManagerComponent
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetTheMainHUD();

	GetWorld()->SpawnActor<AUnitBase>(UnitClllas, GetActorLocation(), GetActorRotation());		

	SpawnFormationGrid();
}

void APlayerCharacter::GetTheMainHUD()
{
	///REFACTOR OR DELETE IF ITS NOT IN USE


	//Get player controller and from there get Hud (AMainHud)
	GridPlayerController = Cast<AGridPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (GridPlayerController)
	{
		MainHud = Cast<AMainHud>(GridPlayerController->GetHUD());
		if (GridPlayerController->ResourceWidgetReference)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hajde"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Haaajde"));

		}
	}
}

void APlayerCharacter::SpawnFormationGrid()
{
	if (GetWorld() && gridRepresentativeClass)
	{
		for (int32 i = 0; i < columns * rows; i++)
		{
			AGridRepresentative* grid = GetWorld()->SpawnActor<AGridRepresentative>(gridRepresentativeClass, GetActorLocation(), GetActorRotation());
			if (grid) horizontalFormationGrids.AddUnique(grid);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateCameraLocation();

	DedectGrid();
}

void APlayerCharacter::UpdateCameraLocation()
{
	if (!GridPlayerController || !Camera) return;

	FVector2D mousePosition;
	GridPlayerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	int32 viewportSizeX;
	int32 viewportSizeY;
	GridPlayerController->GetViewportSize(viewportSizeX, viewportSizeY);

	if (mousePosition.Y > viewportSizeY - cameraEdgeOffset) AddActorLocalOffset(FVector(-cameraMoveSpeed, 0,0 ));
	else if (mousePosition.Y < cameraEdgeOffset) AddActorLocalOffset(FVector(cameraMoveSpeed, 0, 0));

	if (mousePosition.X > viewportSizeX - cameraEdgeOffset) AddActorLocalOffset(FVector(0, cameraMoveSpeed, 0));
	else if (mousePosition.X < cameraEdgeOffset) AddActorLocalOffset(FVector(0, -cameraMoveSpeed, 0));
}

#pragma region InputBindings

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (!PlayerInputComponent) { return; }
	
	PlayerInputComponent->BindAction(FName("DedectGrid"), EInputEvent::IE_Pressed, this, &APlayerCharacter::AttachBuildingToGrid);

	PlayerInputComponent->BindAction(FName("SelectUnits"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction(FName("SelectUnits"), EInputEvent::IE_Released, this, &APlayerCharacter::LeftMouseButtonReleased);

	PlayerInputComponent->BindAction(FName("UnitBehaviour"), EInputEvent::IE_Pressed, this, &APlayerCharacter::UnitBehaviour);
	
}

void APlayerCharacter::AttachBuildingToGrid()
{
	

	//If inventory is open and there is not current building(not building selected) then building cannot be attached to grid
	if (!currentBuilding) { return; }
	if (bIsInventoryOpen ) { return; }
	/*Check if current building can occupy more then one grid and if one of the neighbours is occupied then building cannot be attached to grid*/
	const bool bIsOneOfTheNeighboursOccupied = currentBuilding->GetOccupiesNeighbours() && (bNorthNeighbourIsOccupied || bSouthNeighbourIsOccupied || bEastNeighbourIsOccupied || bWesthNeighbourIsOccupied);
	if (bIsOneOfTheNeighboursOccupied) { return; }

	// Check if current grid does exists and is not occupied
	if (const bool bIsGridNotOccupied = currentGrid && !currentGrid->IsOccupied())
	{
		//spawn the building class at grid location and with grid rotation
		ABaseBuilding* placedBuilding = GetWorld()->SpawnActor<ABaseBuilding>(currentBuilding->GetClassRepresentative(), currentGrid->GetActorLocation(), currentBuilding->GetActorRotation());
		if (!placedBuilding) { return; }
		//destroy unplaced building(the building to determine placed buildings location)
		if (currentBuilding) currentBuilding->Destroy();
		//When Building is placed start the building functionality
		placedBuilding->BuildingFunctionality();

		//Occupy the grid
		currentGrid->OccupyGrid();

		//And set the current building to null so later on another building can be selected and placed
		currentBuilding = nullptr;
	}
}

void APlayerCharacter::LeftMouseButtonPressed()
{
	bIsHoldingLeftMouse = true;
	//if its holding the leftMouse button then start to draw a rectangle (its about selection the units)
	if (bIsHoldingLeftMouse && MainHud)MainHud->StartDraw();

}

void APlayerCharacter::LeftMouseButtonReleased()
{
	bIsHoldingLeftMouse = false;
	//if the LeftMouse button is released then stop drawing the rectangle
	if (!bIsHoldingLeftMouse && MainHud)
		MainHud->StopDraw();
}

void APlayerCharacter::UnitBehaviour()
{
	//Offset between each formation grid
	rightOffset = 0.f;

	//If MainHud and GridPlayerController doesnt exists then do nothing
	if (!MainHud && !GridPlayerController) { return; }
	//If there is no unit selected do nothing
	if (MainHud->GetSelectedUnitComponents().Num() == 0) { return; }
	
	//Convert current 2d mouse position to 3d world space position and direction , so the units can move to a selected location
	FVector mousePosition;
	FVector mouseDirection;
	GridPlayerController->DeprojectMousePositionToWorld(mousePosition, mouseDirection);

	//Declare a HitResult variable to store information about the hit
	FHitResult hitResult;
	//Calculate the starting position of the line trace from the mouse position
	const FVector startPosition = mousePosition;
	//Calculate the end position of the line trace by extending it in the direction of the mouse, 2000 units away from the starting position
	const FVector endPosition = mousePosition + mouseDirection * 2000.f;
	//Perform a line trace from the starting position to the end position
	const bool bHasHit = GetWorld()->LineTraceSingleByChannel(hitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility);

	if (!bHasHit) { 
		UE_LOG(LogTemp, Warning, TEXT("Line trace didnt hit"));
		return; }

	//if line trace has hit, create an array of AUnitBase based on MainHud's selected unit array----------
	TArray<UUnitComponent*> selectedUnitComponents = MainHud->GetSelectedUnitComponents();

	//change formation direction(horizontal formation,or vertical formation)
	for (int32 k = 0; k < horizontalFormationGrids.Num(); k++)
	{
		const FVector endPos = hitResult.ImpactPoint + formationDirection * rightOffset;
		horizontalFormationGrids[k]->SetActorLocation(endPos);
		rightOffset += 300.f;
	}

	for (int32 i = 0; i < selectedUnitComponents.Num(); i++)
	{
		if (!selectedUnitComponents[i]->GetIsUnitSelected()) { return; }
		bWasSelected = true;

		if (bWasSelected)
		{
			selectedUnitComponents[i]->NewMove(horizontalFormationGrids[i]->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Player move"));
		}
	}
}

#pragma endregion InputBindings

void APlayerCharacter::DedectGrid()
{
	if (!currentBuilding) { return; }

	if (GetWorld())
	{
		FHitResult HitResult;
		//Get mouse position
		FVector worldLocation;
		FVector worldDirection;
		GridPlayerController->DeprojectMousePositionToWorld(worldLocation, worldDirection);

		const FVector startPosition = worldLocation;
		const FVector endPosition = startPosition + worldDirection * TraceLength;

		//Trace a line from startPosition to endPosition
		const bool hasHit = GetWorld()->LineTraceSingleByChannel(HitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility);
		if (hasHit)
		{
			//If line trace has hit , then set the currentGrid variable to actor from the hitResult
			currentGrid = Cast<AGridRepresentative>(HitResult.GetActor());
			
			//Check if current grid exists and is not occupied
			if (currentGrid && !currentGrid->IsOccupied())
			{
				//set the current building location to the grid location
				currentBuilding->SetActorLocation(currentGrid->GetActorLocation());
			}

		}
		else
			currentGrid = nullptr; // if trace didnt hit anything then set curretGrid to null,the building will be placed in the last grid
	}
}

void APlayerCharacter::SelectBuilding(TSubclassOf<ABaseBuilding> NewBuilding)
{
	if (!NewBuilding && !GetWorld()) { return; }
	currentBuilding = GetWorld()->SpawnActor<ABaseBuilding>(NewBuilding);
}

void APlayerCharacter::SetFormationToVerticalFormation()
{
	formationDirection = GetActorForwardVector();
}

void APlayerCharacter::SetFormationToHorizontalFormation()
{
	formationDirection = GetActorRightVector();
}