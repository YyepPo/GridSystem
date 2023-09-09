// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/BuildingWidget.h"
#include "HouseWidget.generated.h"

/**
 * 
 */
UCLASS()
class GRIDSYSTEM_API UHouseWidget : public UBuildingWidget
{
	GENERATED_BODY()
	
public:
	virtual void ButtonClickedEvent() override;
protected:

};
