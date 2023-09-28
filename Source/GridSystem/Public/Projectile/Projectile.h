#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class AObjectPooling;

UCLASS()
class GRIDSYSTEM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	void Activate();
	void Deactivate();

	UFUNCTION(BlueprintCallable)
		void SetObjectPoolingActor(AObjectPooling* newPoolingActor) { objectPoolingActor = newPoolingActor; }

	bool bIsActive = false;
	UFUNCTION(BlueprintImplementableEvent)
		void OnActivate();
	FORCEINLINE void SetProjectileDamage(float damage) { damageAmount = damage; }
protected:
	virtual void BeginPlay() override;
	
private:
	FTimerHandle timerHandle;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		AObjectPooling* objectPoolingActor;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	void DealDamage(AActor* damagedActor);

	UPROPERTY(EditAnywhere)
		float damageAmount = 1.f;
	UPROPERTY(EditAnywhere)
		FName targetTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* staticMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* boxCollider;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* projectileMovementComp;

};
