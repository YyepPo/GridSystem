#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TypeOfResource.h"
#include "Units/IsAttackableInterface.h"
#include "ResourceBase.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class UResourceManager;
class UResourceDataAsset;

UCLASS()
class GRIDSYSTEM_API AResourceBase : public AActor,public IIsAttackableInterface
{
	GENERATED_BODY()
	
public:	
	AResourceBase();

	virtual void IsAttackAble() override;
	
	bool IsResourceDestroyed();
protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

#pragma region Components

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* staticMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		UBoxComponent* boxCollider;

#pragma endregion Components

	//Resource Type
	UPROPERTY(VisibleAnywhere, Category = "Resource Type")
		ETypeResource resourceType;

	/// Health
	UPROPERTY(EditAnywhere, Category = "Health")
		float maxHealth;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		float health;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		bool bIsDestroyed;

	/// VFX
	UPROPERTY(EditAnywhere, Category = "VFX")
		float systemZOffset;
	UPROPERTY()
		UNiagaraSystem* onHitVFX;
	UPROPERTY()
		UNiagaraSystem* onDestroyedVFX;

	/// Sounds
	UPROPERTY()
		USoundBase* hitSound;
	UPROPERTY()
		USoundBase* destroyedSound;

private:
	void LoadDataAsset();

	void PlaySoundAndVFX(UNiagaraSystem* particle, USoundBase* sound);

	UPROPERTY()
		UResourceManager* resourceManager;
	UPROPERTY()
		class AResource* resource;

	/// Data Asset
	UPROPERTY(EditAnywhere, Category = "MyData")
		TSoftObjectPtr<UResourceDataAsset> resourceDataAsset;
	UPROPERTY()
		UResourceDataAsset* loadedDataAsset;

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE void SetResourceManager(AResource* newResource) { resource = newResource; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE AResource* GetResource() const { return resource; }
	FORCEINLINE bool GetIsResourceDestroyed() const { return bIsDestroyed; }
	FORCEINLINE ETypeResource GetResourceType() const { return resourceType; }
};
