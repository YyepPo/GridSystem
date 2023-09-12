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

void AFriendlyArcher::MoveToTarget(AActor* target)
{
	Super::MoveToTarget(target);
	UE_LOG(LogTemp, Warning, TEXT("Archer move"));
}

void AFriendlyArcher::OnMoveComplete()
{
	if (!enemyInfantryTarget) { return; }
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), enemyInfantryTarget->GetActorLocation());
	SetActorRotation(lookRotation);
}

void AFriendlyArcher::LaunchArrow()
{
	UE_LOG(LogTemp, Warning, TEXT("Launch arrow"));
	if (!arrowProjectile) { return; }

	if (enemyInfantryTarget && enemyInfantryTarget->IsUnitDead())
	{
		
	}
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), enemyInfantryTarget->GetActorLocation());
	SetActorRotation(lookRotation);
	currentArrow = GetWorld()->SpawnActor<AProjectile>(arrowProjectile, arrowSpawnPoint->GetComponentLocation(), arrowSpawnPoint->GetComponentRotation());
	currentArrow->SetOwner(this);
	currentArrow->SetInstigator(this);
}
