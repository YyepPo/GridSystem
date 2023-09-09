#include "Units/FriendlyArcher.h"
#include "Projectile/Projectile.h"

#include "Kismet/KismetMathLibrary.h"

#include "Units/EnemyInfatry.h"

AFriendlyArcher::AFriendlyArcher():
	arrowSpawnPoint {CreateDefaultSubobject<USceneComponent>(TEXT("Arrow spawn point"))}
{
	arrowSpawnPoint->SetupAttachment(GetRootComponent());
}

void AFriendlyArcher::BeginPlay()
{
	Super::BeginPlay();
}

void AFriendlyArcher::LaunchArrow()
{
	UE_LOG(LogTemp, Warning, TEXT("Launch arrow"));
	if (!arrowProjectile) { return; }

	if (enemyInfantryTarget && enemyInfantryTarget->IsUnitDead())
	{
		//if current target is dead spawn an arrow destroy the previous one
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), overlappingEnemyInfatries[0]->GetActorLocation());
		SetActorRotation(lookRotation);
	}

	currentArrow = GetWorld()->SpawnActor<AProjectile>(arrowProjectile, arrowSpawnPoint->GetComponentLocation(), arrowSpawnPoint->GetComponentRotation());
	currentArrow->SetOwner(this);
	currentArrow->SetInstigator(this);
}
