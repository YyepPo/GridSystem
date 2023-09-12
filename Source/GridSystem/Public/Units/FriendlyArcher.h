#pragma once

#include "CoreMinimal.h"
#include "Units/FriendlyInfantry.h"
#include "FriendlyArcher.generated.h"

class AProjectile;

UCLASS()
class GRIDSYSTEM_API AFriendlyArcher : public AFriendlyInfantry
{
	GENERATED_BODY()
	
public:
	AFriendlyArcher();

	UFUNCTION(BlueprintCallable)
		void LaunchArrow();
protected:
	virtual void BeginPlay() override;
	virtual void MoveToTarget(AActor* target) override;
	virtual void OnMoveComplete() override;
private:
	UPROPERTY(EditAnywhere,Category = "Projectile")
		TSubclassOf<AProjectile> arrowProjectile;
	UPROPERTY()
		AProjectile* currentArrow;
	
	UPROPERTY(EditAnywhere)
		USceneComponent* arrowSpawnPoint;
};
