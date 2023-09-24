#include "ObjectPooling/ObjectPooling.h"
#include "Projectile/Projectile.h"
AObjectPooling::AObjectPooling()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AObjectPooling::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < 10; ++i)
	{
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, GetActorLocation(), GetActorRotation());
		if(projectile)
		{
			projectilePool.AddUnique(projectile);
			projectile->Deactivate();
		}
	}
}

AProjectile* AObjectPooling::GetPooledProjectile(AActor* newOwner)
{
	for (AProjectile* projectile : projectilePool)
	{
		if(!projectile->bIsActive)
		{
			projectile->SetOwner(newOwner);
			projectile->SetInstigator(Cast<APawn>(newOwner));
			projectile->Activate();
			return projectile;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No available object in the pool"));
	return nullptr;
}

void AObjectPooling::ReturnPooledObject(AProjectile* projectile)
{
	projectile->Deactivate();
}