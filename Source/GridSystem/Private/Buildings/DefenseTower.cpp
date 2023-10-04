#include "Buildings/DefenseTower.h"

#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"

#include "NewUnit/UnitComponent.h"

#include "HUD/HealthBarWidget.h"

#include "Grid/GridRepresentative.h"

#include "Kismet/KismetMathLibrary.h"

#include "ObjectPooling/ObjectPooling.h"

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

	//This prevents the tower to attack at every frame
	if (towerState != ETowerState::ETS_Attacking)
	{
		StartToAttack();
	}
}

float ADefenseTower::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	healthComponent->TakeDamage(DamageAmount);
	if(healthBarWidget)healthBarWidget->UpdateHealthBarPercentage(healthComponent->GetHealthPercentage());

	return DamageAmount;
}

void ADefenseTower::OnHit(float damageAmount)
{
	//Blueprint implementable event(check blueprint)
	OnTowerHit();
}

bool ADefenseTower::OnDeath()
{
	if (healthComponent->GetCurrentHealthAmount() <= 0)
	{
		//BlueprintImplementableEvent
		OnTowerDestroyed();

		//After the tower's destruction, unoccupy the grid it once occupied.
		FHitResult hitResult;
		const FVector startPos = GetActorLocation();
		const FVector endPos = startPos + GetActorUpVector() * 50.f;
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		if (const bool hasHit = GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, ECC_Visibility, param))
		{
			AGridRepresentative* occupiedGrid = Cast<AGridRepresentative>(hitResult.GetActor());
			if (occupiedGrid) occupiedGrid->UnOccupyGrid();
		}
		return true;
	}
	return false;
}

void ADefenseTower::OnTowerDestroyedDelegateHandler()
{
	Destroy();
}

void ADefenseTower::StartToAttack()
{
	//check if closest target is dead
	if (!closestEnemy) { return; }
	targetUnitComponent = (targetUnitComponent == nullptr) ? Cast<UUnitComponent>(closestEnemy->GetComponentByClass(unitComponentClass)) : targetUnitComponent;
	if (!targetUnitComponent) { return; }
	if (targetUnitComponent->GetUnitDead())
	{
		overlapingEnemies.Remove(closestEnemy);
		closestEnemy = nullptr;
		targetUnitComponent = nullptr;
		if (overlapingEnemies.Num() == 0) { return; }
		closestEnemy = overlapingEnemies[0];
	}

	const FRotator lookDirection = UKismetMathLibrary::FindLookAtRotation(projectileSpawnPoint->GetComponentLocation(), closestEnemy->GetActorLocation());
	projectileSpawnPoint->SetRelativeRotation(lookDirection);

	if(objectPool)
	{
		objectPool->GetPooledProjectile(this,projectileSpawnPoint->GetComponentLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Object pool is not available in %s"),*GetActorNameOrLabel());
	}

	//set tower state to attacking so the tower doesn't attack multiple times
	towerState = ETowerState::ETS_Attacking;

	GetWorld()->GetTimerManager().SetTimer(attackTimerRateHandle,this,&ADefenseTower::BackToUnocuppiedState,attackRate,false);
}

void ADefenseTower::BackToUnocuppiedState()
{
	//Set state to ETS_Unoccupied so the tower doesn't attack rapidly
	towerState = ETowerState::ETS_Unoccupied;
}

void ADefenseTower::OnBoxColliderClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	Super::OnBoxColliderClicked(TouchedComponent, ButtonPressed);
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Defense Tower object is beign clicked")));
}

void ADefenseTower::LevelUpFunctionality()
{
	Super::LevelUpFunctionality();

	//On Level up upgrade static mesh and the projectile damage amount
	for (int32 i = 0; i < upgradeData.Num(); i++)
	{
		if (i == buildingLevelData.currentLevel)
		{
			StaticMesh->SetStaticMesh(upgradeData[i].mesh);
			projectileDamage = upgradeData[i].damage;
		}
	}
}

void ADefenseTower::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FName tag = "EnemyTarget";
	if (!OtherActor->ActorHasTag(tag)) { return; }

	overlapingEnemies.AddUnique(OtherActor);
	//if there is no closest enemy and an enemy had overlapped with tower then set that enemy to be the closest enemy
	if(!closestEnemy) closestEnemy = overlapingEnemies[0];
}
