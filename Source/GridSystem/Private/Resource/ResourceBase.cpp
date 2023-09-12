#include "Resource/ResourceBase.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Player/ResourceManager.h"//--
#include "Resource/Resource.h"

#include "DataAssets/ResourceDataAsset.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AResourceBase::AResourceBase() 
{
	PrimaryActorTick.bCanEverTick = false;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Static Mesh")));
	SetRootComponent(staticMesh);

	boxCollider = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Box Collider")));
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
	if (resourceDataAsset.IsValid())
	{
		loadedDataAsset = resourceDataAsset.LoadSynchronous();
		if (loadedDataAsset)
		{
			staticMesh->SetStaticMesh(loadedDataAsset->staticMesh);
			resourceType = loadedDataAsset->resourceType;
			onHitVFX = loadedDataAsset->onHitVFX;
			onDestroyedVFX = loadedDataAsset->destroyedVFX;
			hitSound = loadedDataAsset->hitSound;
			destroyedSound = loadedDataAsset->destroyedSound;
		}
	}
}

void AResourceBase::IsAttackAble()
{

}

float AResourceBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//When this component gets hit, player gains a x amount of resource 
	loadedDataAsset->OnResourceHit(resource);

	if (IsResourceDestroyed()) { return 0; }
	//Prevents health amount to go below zero
	health = FMath::Max(health - DamageAmount, 0);

	//play a hit vfx and sound
	PlaySoundAndVFX(onHitVFX, hitSound);

	return DamageAmount;
}

bool AResourceBase::IsResourceDestroyed()
{
	//if resource's health is below zero destroy it
	if (health <= 0)
	{
		bIsDestroyed = true;
		PlaySoundAndVFX(onDestroyedVFX, destroyedSound);
		Destroy();
		return true;
	}
	return false;
}

void AResourceBase::PlaySoundAndVFX(UNiagaraSystem* particle, USoundBase* sound)
{
	if (!particle && !sound) { return; }

	//vfx
	const FVector systemSpawnLocation = GetActorLocation() + GetActorUpVector() * systemZOffset;
	if (particle) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, particle, systemSpawnLocation);

	//sound
	if (sound)UGameplayStatics::PlaySoundAtLocation(this, sound, GetActorLocation());
}