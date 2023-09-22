#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NewFriendlyUnit.generated.h"

class USphereComponent;
class UDecalComponent;
class UUnitComponent;
class UHealthComponent;

UCLASS()
class GRIDSYSTEM_API ANewFriendlyUnit : public ACharacter
{
	GENERATED_BODY()

public:
	ANewFriendlyUnit();
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UUnitComponent* GetUnitsUnitComponent() const { return unitComponent; }
protected:
	virtual void BeginPlay() override;

private:

#pragma region Components

	UFUNCTION()
		void TargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TargetOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UUnitComponent* unitComponent;
	UPROPERTY(EditAnywhere,meta = (AllowPrivateAccess = "true"))
		UHealthComponent* healthComponent;
	UPROPERTY(EditAnywhere,meta = (AllowPrivateAccess = "true"))
		USphereComponent* targetDedectCollider;
	UPROPERTY(EditAnywhere, Category = "Decal")
		UDecalComponent* selectionDecalComponent;

#pragma endregion Components
};
