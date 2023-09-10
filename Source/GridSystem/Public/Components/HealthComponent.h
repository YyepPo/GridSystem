#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTowerDestroyed);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIDSYSTEM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	FOnTowerDestroyed OnTowerDestroyedDelegate;

	void TakeDamage(float damageAmount);

	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetHealthPercentage() {return heathPercentage;}
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FORCEINLINE float GetCurrentHealthAmount() const {return currentHealth;}
	UFUNCTION()
		FORCEINLINE bool IsTowerDestroyed() const {return bIsTowerDestroyed;}

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float maxHealth;
	UPROPERTY(VisibleAnywhere)
		float currentHealth;
	UPROPERTY(VisibleAnywhere)
		float heathPercentage;
	UPROPERTY()
		bool bIsTowerDestroyed;
};
