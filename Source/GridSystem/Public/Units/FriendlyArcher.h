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
private:
	UPROPERTY(EditAnywhere,Category = "Projectile")
		TSubclassOf<AProjectile> arrowProjectile;
	UPROPERTY()
		AProjectile* currentArrow;
	
	UPROPERTY(EditAnywhere)
		USceneComponent* arrowSpawnPoint;
};
