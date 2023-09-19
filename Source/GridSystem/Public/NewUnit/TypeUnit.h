#pragma once

UENUM(BlueprintType)
enum class ETypeUnit : uint8
{
	ETU_Miner UMETA(DisplayName = "Miner"),
	ETU_NormalInfantry UMETA(DisplayName = "NormalInfatry"),
	ETU_Archer UMETA(DisplayName = "Archer"),
	ETU_Cavalry UMETA(DisplayName = "Cavalry"),

	ETU_NONE UMETA(DisplayName = "None")
	
};
