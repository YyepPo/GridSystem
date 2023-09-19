#include "Units/InfantryBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HitInterface.h"
#include "Units/FriendlyInfantry.h"
#include "Buildings/DefenseTower.h"
AInfantryBase::AInfantryBase()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereCollider = CreateDefaultSubobject<USphereComponent>(FName(TEXT("Sphere Collider")));
	sphereCollider->SetupAttachment(GetRootComponent());
}

void AInfantryBase::BeginPlay()
{
	Super::BeginPlay();

	currentHealthAmount = maxHealth;
}

void AInfantryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AInfantryBase::CanAttack(AActor* target)
{
	return !isDead && IsInAttackRange(target) && !IsAttacking();
}

void AInfantryBase::StartToAttack()
{
	unitState = EUnitState::EUS_Attacking;
}

void AInfantryBase::CheckForClosestTargett(AInfantryBase*& unitTarget,TArray<AInfantryBase*>& overlapingUnits)
{
	if (overlapingUnits.Num() == 0) { return; }
	unitTarget = nullptr;
	if (!unitTarget)
	{
		float minDistance = 9999.f;
		for (int32 i = 0; i < overlapingUnits.Num(); i++)
		{
			float distance = (overlapingUnits[i]->GetActorLocation() - GetActorLocation()).Size();
			if (distance < minDistance)
			{
				minDistance = distance;
				unitTarget = overlapingUnits[i];
				SetRotationTowardsTarget(unitTarget);
				MoveToTargetInterface(unitTarget);
				MoveToTarget(unitTarget);
				UE_LOG(LogTemp, Warning, TEXT("closest enemy to %s is : %s"), *GetActorNameOrLabel(), *unitTarget->GetActorNameOrLabel());
				break;
			}
		}
	}
}

void AInfantryBase::MoveToTarget(AActor* target)
{
	Super::MoveToTarget(target);
}

float AInfantryBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	currentHealthAmount -= DamageAmount;
	return DamageAmount;
}

void AInfantryBase::OnAttackDealDamageAnimNotify()
{
	OnAttackDealDamage();                                             
}

void AInfantryBase::OnAttackDealDamage()
{

}

void AInfantryBase::DamageBehaviour(AActor* target)
{
	//if (target)
	//{
	//	UGameplayStatics::ApplyDamage(target,
	//		attackDamage,
	//		GetController(),
	//		this,
	//		UDamageType::StaticClass());
	//
	//	IHitInterface* hit = Cast<IHitInterface>(target);
	//	if (hit)
	//	{
	//		hit->OnHit();
	//		if (hit->OnDeath())
	//		{
	//			target = nullptr;
	//		}
	//	}
	//}
}

bool AInfantryBase::IsTargetDead(IHitInterface* hitInterface)
{
	return false;
}

bool AInfantryBase::DedectTarget(FHitResult& hitResult)
{
	const FVector sphereLocation = GetActorLocation();
	const FCollisionShape shape = FCollisionShape::MakeSphere(sphereRadiuss);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	const bool bHasHit = GetWorld()->SweepSingleByChannel(hitResult,
		sphereLocation, sphereLocation,
		FQuat::Identity,
		ECC_GameTraceChannel8,
		shape, params);

	if (bHasHit) { return true; }
	return false;
}

void AInfantryBase::OnCollisionBehaviour(AInfantryBase& actor, TArray<AInfantryBase*>& actors,AInfantryBase* targetUnit)
{
	actors.AddUnique(&actor);
	targetUnit = actors[0];
	if (targetUnit) MoveToTarget(targetUnit);
	for (int32 i = 0; i < actors.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *actors[i]->GetActorNameOrLabel());
	}
}

void AInfantryBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AInfantryBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

