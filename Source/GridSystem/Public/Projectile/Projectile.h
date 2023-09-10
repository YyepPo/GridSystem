#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class IHitInterface;

UCLASS()
class GRIDSYSTEM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	FORCEINLINE void SetProjectileDamage(float damage) { damageAmount = damage; }
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	/// <summary>
	/// Damage
	/// </summary>
	void DealDamage(AActor* damagedActor);

	IHitInterface* hitInterface;

	float damageAmount = 1.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* staticMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* boxCollider;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* projectileMovementComp;

};
