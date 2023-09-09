#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UnitController.generated.h"

class AUnitBase;

/// <summary>
/// This class is not in use
/// </summary>

UCLASS()
class GRIDSYSTEM_API AUnitController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void AddToSelection(AUnitBase* UnitToAdd);
	UFUNCTION()
		void RemoveFromSelection(AUnitBase* UnitToRemove);
private:
	UPROPERTY(VisibleAnywhere, Category = "Units")
		TArray<AUnitBase*> SelectedUnits;

};
