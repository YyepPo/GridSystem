// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GridSystemGameModeBase.generated.h"


UCLASS()
class GRIDSYSTEM_API AGridSystemGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnMotherBuildingDestroyedHandler();
protected:
	virtual void BeginPlay() override;

private:
	void SpawnGrid() const;
	UPROPERTY(EditAnywhere, Category = "Game Lost")
		TSubclassOf<UUserWidget> widgetClass;
	UPROPERTY(EditAnywhere, Category = "Wave")
		TSubclassOf<AActor> motherBuildingClass;
};
