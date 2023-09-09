#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource.generated.h"

USTRUCT(BlueprintType)
struct FResources
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		int32 currentPeopleAmount;
	UPROPERTY(EditAnywhere)
		int32 currentCoinAmount;
	UPROPERTY(EditAnywhere)
		int32 currentWoodAmount;
	UPROPERTY(EditAnywhere)
		int32 currentStoneAmount;
};

class UResourceWidget;

UCLASS()
class GRIDSYSTEM_API AResource : public AActor
{
	GENERATED_BODY()
	
public:	
	AResource();

	void AddPeople(int peopleAmount);
	void RemovePeople(int peopleAmount);

	void AddCoin(int coinAmount);
	void RemoveCoin(int coinAmount);

	void AddWood(int woodAmount);
	void RemoveWood(int woodAmount);

	void AddStone(int stoneAmount);
	void RemoveStone(int stoneAmount);

	FORCEINLINE int32 GetPoepleAmount() const { return resources.currentPeopleAmount; }
	FORCEINLINE int32 GetCoinAmount() const { return resources.currentCoinAmount; }
	FORCEINLINE int32 GetWoodAmount() const { return resources.currentWoodAmount; }
	FORCEINLINE int32 GetStoneAmount() const { return resources.currentStoneAmount; }

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UResourceWidget> resourceWidgetClass;
	UResourceWidget* resourceWidget;

	UPROPERTY(EditAnywhere)
		FResources resources;
};
