#include "Units/EnemyInfatry.h"
#include "Units/FriendlyInfantry.h"
#include "Units/DeathInterface.h"
#include "Units/InfantryBase.h"

#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Buildings/DefenseTower.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));
#define PrintNameToLog(message) UE_LOG(LogTemp,Warning,TEXT("%s"),*message);

AEnemyInfatry::AEnemyInfatry()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyInfatry::BeginPlay()
{
	Super::BeginPlay();

	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AEnemyInfatry::SphereOverlapBegin);
	sphereCollider->OnComponentEndOverlap.AddDynamic(this, &AEnemyInfatry::SphereOverlapEnd);
}

void AEnemyInfatry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Check if this unit is dead
	if (IsUnitDead()) { return; }

	if (IsTowerTargetValid())
	{
		if (CanAttack(towerTarget)) StartToAttack();
		return;
	}

	if (!IsTargetValid())
	{
		if(overlapingInfantry.Num() > 0) CheckForClosestTargett(currentFriendlyUnitTarget, overlapingInfantry);
		return;
	}
	CanAttack(currentFriendlyUnitTarget) ? StartToAttack() : MoveToTargetInterface(currentFriendlyUnitTarget);
}

void AEnemyInfatry::MoveToTarget(AActor* target)
{
	Super::MoveToTarget(target);
	currentFriendlyUnitTarget = currentFriendlyUnitTarget == nullptr ? Cast<AFriendlyInfantry>(currentFriendlyUnitTarget) : currentFriendlyUnitTarget;
}

bool AEnemyInfatry::IsTargetValid()
{
	//check if tower is still alive, if is not then look for the closest target
	return currentFriendlyUnitTarget != nullptr && !currentFriendlyUnitTarget->IsUnitDead();
}

bool AEnemyInfatry::IsTowerTargetValid()
{
	return towerTarget != nullptr;
}

void AEnemyInfatry::StartToAttack()
{
	Super::StartToAttack();
	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AEnemyInfatry::Attack, attackRate);
}

void AEnemyInfatry::OnAttackDealDamage()
{
	if (towerTarget)
	{
		DamageBehaviour(towerTarget);
		IHitInterface* hitInterface = Cast<IHitInterface>(towerTarget);     //Temporary
		if (hitInterface && hitInterface->OnDeath()) towerTarget = nullptr; //Temporary
	}
	else if(currentFriendlyUnitTarget)
		DamageBehaviour(currentFriendlyUnitTarget);

}

void AEnemyInfatry::IsAttackAble()
{
}

bool AEnemyInfatry::IsTargetDead(IHitInterface* hitInterface)
{
	Super::IsTargetDead(hitInterface);

	if (hitInterface && hitInterface->OnDeath())
	{
		CheckForClosestTargett(currentFriendlyUnitTarget, overlapingInfantry);
		return true;
	}
	return false;
}

void AEnemyInfatry::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInfantryBase* targetUnitt = Cast<AInfantryBase>(OtherActor);
	if (targetUnitt)
	{
		OnCollisionBehaviour(*targetUnitt, overlapingInfantry,currentFriendlyUnitTarget);
	}

	ADefenseTower* tower = Cast<ADefenseTower>(OtherActor);
	if (tower)
	{
		towerTarget = tower;
		MoveToTarget(towerTarget);
	}
}

void AEnemyInfatry::SphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	overlapingInfantry.Remove(Cast<AFriendlyInfantry>(OtherActor));
}
