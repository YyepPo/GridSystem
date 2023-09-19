#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "HitInterface.h"
#include "MotherBuilding.generated.h"

class UHealthBarWidget;
class UHealthComponent;

UCLASS()
class GRIDSYSTEM_API AMotherBuilding : public ABaseBuilding,public IHitInterface
{
	GENERATED_BODY()

public:
	AMotherBuilding();

	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetUpHealthWidget(UHealthBarWidget* newHealthBarWidget) { healthBarWidget = newHealthBarWidget; }


	virtual void OnHit(float damageAmount) override;
	virtual bool OnDeath() override;
protected:
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UHealthComponent* healthComponent;
	UPROPERTY()
		UHealthBarWidget* healthBarWidget;
};
