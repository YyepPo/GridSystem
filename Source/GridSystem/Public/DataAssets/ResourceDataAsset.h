#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TypeOfResource.h"
#include "ResourceDataAsset.generated.h"

class UNiagaraSystem;

UCLASS()
class GRIDSYSTEM_API UResourceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void OnResourceHit(class AResource* resourceManager);

	UPROPERTY(EditAnywhere)
		FString name;
	UPROPERTY(EditAnywhere)
		FString description;
	UPROPERTY(EditAnywhere)
		UStaticMesh* staticMesh;
	UPROPERTY(EditAnywhere)
		ETypeResource resourceType;
	UPROPERTY(EditAnywhere)
		int32 minResource;
	UPROPERTY(EditAnywhere)
		int32 maxResource;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* onHitVFX;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* destroyedVFX;
	UPROPERTY(EditAnywhere)
		USoundBase* hitSound;
	UPROPERTY(EditAnywhere)
		USoundBase* destroyedSound;
};
