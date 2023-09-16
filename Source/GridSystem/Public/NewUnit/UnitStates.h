#pragma once

UENUM(BlueprintType)
enum class UnitStates : uint8
{
	EUS_Idle UMETA(DisplayName = "Idle"),
	EUS_Attacking UMETA(DisplayName = "Attacking"),
	EUS_UnOcuppied UMETA(DisplayName = "UnOcuppied"),
	EUS_Dead UMETA(DisplayName = "Dead"),
	EUS_NONE UMETA(DisplayName = "None")
};