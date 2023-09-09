#include "PlayerController/UnitController.h"

#include "UnitBase.h"

/// <summary>
/// This class is not in use
/// </summary>

void AUnitController::AddToSelection(AUnitBase* UnitToAdd)
{
	SelectedUnits.AddUnique(UnitToAdd);
}

void AUnitController::RemoveFromSelection(AUnitBase* UnitToRemove)
{
	SelectedUnits.Remove(UnitToRemove);
}
