
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
protected:
	virtual void BeginPlay() override;

private:	
	void InitializeComponents();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxCollider;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere,Category = "Grid")
		EGridState GridState = EGridState::EGS_UnOccupied;
	UPROPERTY(EditAnywhere, Category = "Grid")
		FLinearColor OccupiedColor;
	UPROPERTY(EditAnywhere, Category = "Grid")
		FLinearColor UnOccupiedColor;
public:
	FORCEINLINE bool IsOccupied() { return GridState == EGridState::EGS_Occupied; }
};
