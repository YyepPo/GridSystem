
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridRepresentative.generated.h"

UENUM(BlueprintType)
enum class EGridState : uint8
{
	EGS_UnOccupied UMETA(DisplayName = "UnOccupied"),
	EGS_Occupied UMETA(DisplayName = "Occupied"),
	EGS_NONE UMETA(DisplayName = "None")
};


UCLASS()
class GRIDSYSTEM_API AGridRepresentative : public AActor
{
	GENERATED_BODY()
	
public:	
	AGridRepresentative();

	UFUNCTION(BlueprintCallable)
		void OccupyGrid();
	UFUNCTION(BlueprintCallable)
		void UnOccupyGrid();

	FORCEINLINE bool IsOccupied() const { return GridState == EGridState::EGS_Occupied; }
protected:
	virtual void BeginPlay() override;

private:	
	void ChangeMaterialColor(FLinearColor newOccupiedColor, FLinearColor newUnoccupiedColor) const;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxCollider;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* staticMesh;

	UPROPERTY(VisibleAnywhere,Category = "Grid")
		EGridState GridState = EGridState::EGS_UnOccupied;
	UPROPERTY(EditAnywhere, Category = "Grid")
		FLinearColor occupiedColor;
	UPROPERTY(EditAnywhere, Category = "Grid")
		FLinearColor unOccupiedColor;
};
