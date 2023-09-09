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

	void TakeDamage(float damageAmount);

	FOnTowerDestroyed OnTowerDestroyedDelegate;

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
		bool bIsTowerDestroyed;
};
