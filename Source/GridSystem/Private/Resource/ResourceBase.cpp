#include "Resource/ResourceBase.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

#include "DataAssets/ResourceDataAsset.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/ObjectLibrary.h"

AResourceBase::AResourceBase() :
	staticMesh { CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh"))) },
	boxCollider { CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider"))) }
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(staticMesh);

	boxCollider->SetupAttachment(GetRootComponent());
}


void AResourceBase::BeginPlay()
{
	Super::BeginPlay();
	LoadDataAsset();
	health = maxHealth;
}

void AResourceBase::LoadDataAsset()
{
	const FString assetPath = assetPathString;

	// Load the UObjectLibrary for the specified asset path
	UObjectLibrary* objectLibrary = UObjectLibrary::CreateLibrary(UResourceDataAsset::StaticClass(), false, true);
	objectLibrary->LoadAssetDataFromPath(assetPath);
	objectLibrary->LoadAssetsFromPath(assetPath);
	TArray<FAssetData> assetDataList;
	objectLibrary->GetAssetDataList(assetDataList);

	if (assetDataList.Num() > 0)
	{
		loadedDataAsset = Cast<UResourceDataAsset>(assetDataList[0].GetAsset());
		if (loadedDataAsset)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *loadedDataAsset->name);
			staticMesh->SetStaticMesh(loadedDataAsset->staticMesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to cast the loaded asset to UResourceDataAsset"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No assets found at path: %s"), *assetPath);
	}
}

float AResourceBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//When component gets hit, player gains a x amount of resource
	if(loadedDataAsset) loadedDataAsset->OnResourceHit(resource);

	if (IsResourceDestroyed()) { return 0; }

	//Prevents health amount to go below zero
	health = FMath::Max(health - DamageAmount, 0);

	//play a hit vfx and sound
	if(loadedDataAsset)PlaySoundAndVFX(loadedDataAsset->hitVFX,loadedDataAsset->hitSound);

	return DamageAmount;
}

bool AResourceBase::IsResourceDestroyed()
{
	//if resource's health is below zero destroy it
	if (health <= 0)
	{
		bIsDestroyed = true;
		if(loadedDataAsset) PlaySoundAndVFX(loadedDataAsset->destroyedVFX,loadedDataAsset->destroyedSound);
		Destroy();
		return true;
	}
	return false;
}

void AResourceBase::OnHit(float damageAmount)
{

}

bool AResourceBase::OnDeath()
{
	return IsResourceDestroyed();
}

void AResourceBase::PlaySoundAndVFX(UNiagaraSystem* particle, USoundBase* sound) const
{
	if (!particle && !sound) { return; }

	//vfx
	const FVector systemSpawnLocation = GetActorLocation() + GetActorUpVector() * systemZOffset;
	if (particle) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, particle, systemSpawnLocation);

	//sound
	if (sound)UGameplayStatics::PlaySoundAtLocation(this, sound, GetActorLocation());
}

ETypeResource AResourceBase::GetResourceType() const
{
	if (loadedDataAsset) return loadedDataAsset->resourceType;

	return ETypeResource::ETR_None;
}
