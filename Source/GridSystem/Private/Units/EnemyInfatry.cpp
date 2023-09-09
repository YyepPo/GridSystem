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

	if (IsUnitDead()) { return; }

	if (!IsTargetValid())
	{
		CheckForClosestTargett(currentFriendlyUnitTarget, overlapingInfantry);
		return;
	}
	CanAttack(currentFriendlyUnitTarget) ? StartToAttack() : MoveToTargetInterface(currentFriendlyUnitTarget);
}

void AEnemyInfatry::MoveToTarget(AActor* target)
{
	Super::MoveToTarget(target);
	currentFriendlyUnitTarget = currentFriendlyUnitTarget == nullptr ? Cast<AFriendlyInfantry>(currentFriendlyUnitTarget) : currentFriendlyUnitTarget;
}

//this one
bool AEnemyInfatry::IsTargetValid()
{
	return currentFriendlyUnitTarget != nullptr && !currentFriendlyUnitTarget->IsUnitDead();
}

//try this one
void AEnemyInfatry::StartToAttack()
{
	Super::StartToAttack();
	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AEnemyInfatry::Attack, attackRate);
}

void AEnemyInfatry::OnAttackDealDamage()
{
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
	if (!targetUnitt) { return; }
	OnCollisionBehaviour(*targetUnitt, overlapingInfantry,currentFriendlyUnitTarget);
}

void AEnemyInfatry::SphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	overlapingInfantry.Remove(Cast<AFriendlyInfantry>(OtherActor));
}
