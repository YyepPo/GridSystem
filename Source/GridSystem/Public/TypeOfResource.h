#pragma once

UENUM(BlueprintType)
enum class ETypeResource : uint8
{
	ETR_Wood UMETA(DisplayName = "Tree"),
	ETR_Stone UMETA(DisplayName = "Stone"),

	ETR_None UMETA(DisplayName = "None")
};