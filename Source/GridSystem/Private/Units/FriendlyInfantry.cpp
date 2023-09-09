#include "Units/FriendlyInfantry.h"
#include "Units/EnemyInfatry.h"
#include "Units/InfantryBase.h"

#include "HitInterface.h"

#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFriendlyInfantry::AFriendlyInfantry()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFriendlyInfantry::BeginPlay()
{
	Super::BeginPlay();
	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AFriendlyInfantry::SphereOverlapBegin);
	sphereCollider->OnComponentEndOverlap.AddDynamic(this, &AFriendlyInfantry::SphereOverlapEnd);
}

void AFriendlyInfantry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsUnitDead()) { return; }
	if (!IsTargetValid()) 
	{ 
		CheckForClosestTargett(enemyInfantryTarget,overlappingEnemyInfatries);
		return; 
	}

	if (!CanAttack(enemyInfantryTarget))
	{
		IsInAttackRange(enemyInfantryTarget) ? navigationAcceptableRadius = attackDistance : navigationAcceptableRadius = 130.f;
		MoveToTarget(enemyInfantryTarget);
	}
	else
	{
		StartToAttack();
	}
}

void AFriendlyInfantry::MoveToTarget(AActor* target)
{
	if (IsInAttackRange(target)) { return; }
	Super::MoveToTarget(target);
	navigationAcceptableRadius = attackDistance;
	enemyInfantryTarget =Cast<AEnemyInfatry>(target);
}

//this one
bool AFriendlyInfantry::IsTargetValid()
{
	return enemyInfantryTarget != nullptr && !enemyInfantryTarget->IsUnitDead();
}

//try this one
void AFriendlyInfantry::StartToAttack()
{
	Super::StartToAttack();
	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AFriendlyInfantry::Attack, attackRate);
}

void AFriendlyInfantry::OnAttackDealDamage()
{
	DamageBehaviour(enemyInfantryTarget);
}

void AFriendlyInfantry::IsAttackAble()
{

}

bool AFriendlyInfantry::IsTargetDead(IHitInterface* hitInterface)
{
	Super::IsTargetDead(hitInterface);
	if (hitInterface && hitInterface->OnDeath())
	{
		CheckForClosestTargett(enemyInfantryTarget, overlappingEnemyInfatries);
		return true;
	}
	return false;
}

void AFriendlyInfantry::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInfantryBase* targetUnit = Cast<AInfantryBase>(OtherActor);
	if (!targetUnit) { return; }
	OnCollisionBehaviour(*targetUnit, overlappingEnemyInfatries, enemyInfantryTarget);
}

void AFriendlyInfantry::SphereOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	overlappingEnemyInfatries.Remove(Cast<AEnemyInfatry>(OtherActor));
}
