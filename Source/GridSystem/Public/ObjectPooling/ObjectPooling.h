#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPooling.generated.h"

class AProjectile;

UCLASS()
class GRIDSYSTEM_API AObjectPooling : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectPooling();

	UFUNCTION()
		AProjectile* GetPooledProjectile(AActor* newOwner,FVector projectileSpawnPoint);

	UFUNCTION()
		void ReturnPooledObject(AProjectile* projectile); 
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> projectileClass;
	UPROPERTY()
		TArray<AProjectile*> projectilePool;
};
