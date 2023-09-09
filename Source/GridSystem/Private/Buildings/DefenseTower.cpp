#include "Buildings/DefenseTower.h"
#include "Components/BoxComponent.h"
#include "Units/EnemyInfatry.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile/Projectile.h"
#include "Components/HealthComponent.h"
#include "DrawDebugHelpers.h"

ADefenseTower::ADefenseTower() :
	enemyDedectionCollider {CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Enemy Dedection Collider"))) },
	projectileSpawnPoint {CreateDefaultSubobject<USceneComponent>(FName(TEXT("Projectile Spawn Point")))},
	healthComponent {CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Components")))}
{

	PrimaryActorTick.bCanEverTick = true;

	enemyDedectionCollider->SetupAttachment(GetRootComponent());
	projectileSpawnPoint->SetupAttachment(GetRootComponent());
}

void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();

	enemyDedectionCollider->OnComponentBeginOverlap.AddDynamic(this, &ADefenseTower::OnBoxBeginOverlap);
	healthComponent->OnTowerDestroyedDelegate.AddDynamic(this, &ADefenseTower::OnTowerDestroyedDelegateHandler);
}

void ADefenseTower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (overlapingEnemies.Num() == 0) { return; }
	if (!closestEnemy) { return; }

	if (towerState != ETowerState::ETS_Attacking)
	{
		StartToAttack();
	}
}

float ADefenseTower::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	healthComponent->TakeDamage(DamageAmount);

	return DamageAmount;
}

void ADefenseTower::OnTowerDestroyedDelegateHandler()
{
	Destroy();
}

void ADefenseTower::StartToAttack()
{
	if (!closestEnemy) { 
		return; }
	if (overlapingEnemies.Num() == 0) { return; }

	//Spawn a projectile in direction to the enemy
	const FVector spawnPoint = projectileSpawnPoint->GetComponentLocation();
	const FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(spawnPoint, closestEnemy->GetActorLocation());
	if (projectileClass)
	{
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, spawnPoint, lookAtRotation);
		if (projectile) projectile->SetOwner(this);
	}

	//set tower state to attacking so the tower doesn't attack multiple times
	towerState = ETowerState::ETS_Attacking;

	GetWorld()->GetTimerManager().SetTimer(attackTimerRateHandle,this,&ADefenseTower::BackToUnocuppiedState,attackRate,false);
}

void ADefenseTower::BackToUnocuppiedState()
{
	//If there are no overlapping enemies then return because downbelow we set the closest enemy 
	//to be equal to the first index of overlappingEnemies, if there are no overlapping enemies then closestEnemy is going to be null
	if (overlapingEnemies.Num() == 0) { return; }

	//set tower state to unoccupied so the tower can start attacking again
	towerState = ETowerState::ETS_Unoccupied;

	if (closestEnemy->IsUnitDead())
	{
		//when unit is dead remove from the array and set it null so the tower doesn't keep attacking the dead unit
		overlapingEnemies.Remove(closestEnemy);
		closestEnemy = nullptr;
		for (int32 i = 0; i < overlapingEnemies.Num(); i++)
		{	
			//Set the closest enemy to be equal to the first index of overlapingEnemies;
			closestEnemy = overlapingEnemies[i];
			break;
		}
	}
}

void ADefenseTower::OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	Super::OnBoxColliderClicked(TouchedComponent, ButtonPressed);
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Defense Tower object is beign clicked")));
}

void ADefenseTower::LevelupFunctionality()
{
	Super::LevelupFunctionality();
	if (staticMeshs.Num() == 0) { return; }

	for (int32 i = 0; i < buildingLevelData.maxLevel + 1; i++)
	{
		if (buildingLevelData.currentLevel == i)
		{
			StaticMesh->SetStaticMesh(staticMeshs[i]);
		}
	}
}

void ADefenseTower::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FName tag = "Enemy";
	if (!OtherActor->ActorHasTag(tag)) { return; }

	overlapingEnemies.AddUnique(Cast<AEnemyInfatry>(OtherActor));
	//if there is no closest enemy and an enemy had overlapped with tower then set that enemy to be the closest enemy
	if(!closestEnemy) closestEnemy = overlapingEnemies[0];
}
