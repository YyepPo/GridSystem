#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

class UNiagaraSystem;

UCLASS()
	class GRIDSYSTEM_API UBuildingDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,Category = "Building")
		bool bOccupiesNeighbours;
	UPROPERTY(EditAnywhere, Category = "Building")
		UNiagaraSystem* buildingPlacedVFX;

	UPROPERTY(EditAnywhere,Category = "Building Function")
		float functionTimeRate;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundBase* placedSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundBase* levelUpSound;

};
