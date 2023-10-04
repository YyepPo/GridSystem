#include "NewUnit/UnitComponent.h"

#include "Components/DecalComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "AIController.h"
#include "AIController/UnitAIController.h"

#include "Projectile/Projectile.h"

#include "HitInterface.h"

#include "NewUnit/NewFriendlyUnit.h"

#include "ObjectPooling/ObjectPooling.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));
#define PrintToScreen(message) 	GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Red, message);

UUnitComponent::UUnitComponent() :
	projectileSpawnPositionComp {CreateDefaultSubobject<USceneComponent>(FName(TEXT("Projectile Spawn Point Comp")))}
{
	PrimaryComponentTick.bCanEverTick = true;
	if(GetOwner() && GetOwner()->GetRootComponent())projectileSpawnPositionComp->SetupAttachment(GetOwner()->GetRootComponent());
}

void UUnitComponent::PlayAMontage(UAnimMontage* montage) const
{
	if (!animInstance || !montage) { return; }
	animInstance->Montage_Play(montage);
}

void UUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	if (unitType == ETypeUnit::ETU_Archer && GetOwner()) ownerUnit = Cast<ANewFriendlyUnit>(GetOwner());

	if (enemyAIController)
			enemyAIController->OnMoveCompletedDelegate.AddDynamic(this, &UUnitComponent::OnMoveCompleteDelegateHandler);
}

void UUnitComponent::OnMoveCompleteDelegateHandler()
{
	if (ownerUnit && !bMoveComplete)
	{
		ownerUnit->SetTargetDedectionCollider(ECollisionEnabled::QueryOnly);
		bMoveComplete = true;
	}
}

void UUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AttackBehaviour();
}

#pragma region Combat

bool UUnitComponent::IsUnitInAttackRangeOf(AActor* target) const
{
	if (!target || !GetOwner()) { return false; }
	//Calculate the distance between target and Owner
	const float distanceBtwTarget = (target->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	//Squaring a distance can be computationally faster than calculating a non-squared distance
	const float squareRootDistance = FMath::Square(distanceBtwTarget);
	return squareRootDistance <= attackDistance * attackDistance;
}

void UUnitComponent::AttackBehaviour()
{
	if (unitState == UnitStates::EUS_Dead || bDeadUnit) { return; }

	if (!currentTarget)
	{
		return;
	}

	//check whenever the current target is dead
	const UUnitComponent* targetsUnitComponent = Cast<UUnitComponent>(currentTarget->GetComponentByClass(unitComponentClass));
	if (targetsUnitComponent && targetsUnitComponent->GetUnitDead())
	{
		overlapingTargets.Remove(currentTarget);
		UE_LOG(LogTemp, Warning, TEXT("%s is dead remove from overlaping targets array"), *currentTarget->GetActorNameOrLabel());
		targetsUnitComponent = nullptr;
		currentTarget = nullptr;
		CheckForClosestTarget();
		return;
	}

	if(CanUnitAttack(currentTarget))
	{
		Attack();
		enemyAIController->ShouldDisableMoveRequest(true);
	}
	else
	{
		NewMove(currentTarget->GetActorLocation());
	}
}

bool UUnitComponent::CanUnitAttack(AActor* target)
{
	unitComponent = (unitComponent == nullptr) ? Cast<UUnitComponent>(target->GetComponentByClass(unitComponentClass)) : unitComponent;

	//check if unit can attack a tower
	const bool canAttackTower = target != nullptr &&
		IsUnitInAttackRangeOf(target) &&
		unitState != UnitStates::EUS_Attacking;

	//check if unit can attack an enemy unit
	const bool canAttackUnit = target != nullptr &&
		IsUnitInAttackRangeOf(target) &&
		unitState != UnitStates::EUS_Attacking &&
		unitComponent && unitComponent->IsUnitInDeadState() == false;

	return canAttackTower || canAttackUnit;
}

void UUnitComponent::Attack()
{
	if (!currentTarget) { return; }

	PlayAMontage(attackMontage);
	unitState = UnitStates::EUS_Attacking;
	GetWorld()->GetTimerManager().SetTimer(attackRateTimerHandle, this, &UUnitComponent::AttackRateHandle, attackRate);
}

void UUnitComponent::AttackRateHandle()
{
	unitState = UnitStates::EUS_UnOcuppied;
}

void UUnitComponent::DealDamageToTargetAnimNotify()
{
	if (!currentTarget)
	{
		PrintToLog("Current target is not valid in DealDamageToTargetAnimNotify")
		return;
	}

	//If target is dead,set the currentTarget to nullptr, so the unit doesn't keep attacking the dead target
	
	IHitInterface* hitInterface = Cast<IHitInterface>(currentTarget);
	if (hitInterface)
	{
		hitInterface->OnHit(attackDamage);
		if (hitInterface->OnDeath())
		{
			overlapingTargets.Remove(currentTarget);
			currentTarget = nullptr;
			return;
		}
	}

	if (unitType != ETypeUnit::ETU_Archer)
	{
		UGameplayStatics::ApplyDamage(currentTarget,
			attackDamage,
			GetOwner()->GetInstigatorController(),
			GetOwner(),
			UDamageType::StaticClass());
	}
	else if (unitType == ETypeUnit::ETU_Archer && projectileClass && projectileSpawnPositionComp)
	{
		if (!currentTarget) { return; }

		if(!objectPooling)
		{
			PrintToLog("There is no object pooling actor");
			return;
		}

		if (GetOwner())
		{
			objectPooling->GetPooledProjectile(GetOwner(),FVector::Zero());
			const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), currentTarget->GetActorLocation());
			GetOwner()->SetActorRotation(lookRotation);
		}
	}

}

void UUnitComponent::OnUnitHit(float damage)
{
	if (OnDeath())
	{
		return;
	}
	PlayAMontage(hitMontage);
	healthAmount -= damage;
}

void UUnitComponent::OnHit(float damageAmount)
{
	if (OnDeath() )
	{
		return;
	}

	PlayAMontage(hitMontage);
	healthAmount -= damageAmount;
}

bool UUnitComponent::OnDeath()
{
	if (healthAmount <= 0 && GetOwner())
	{
		bDeadUnit = true;
		UE_LOG(LogTemp, Warning, TEXT("%s is Dead"), *GetOwner()->GetActorNameOrLabel());
		OnUnitDeath();
		return true;
	}
	return false;
}

void UUnitComponent::OnUnitDeath()
{
	unitState = UnitStates::EUS_Dead;
}

void UUnitComponent::CheckForClosestTarget()
{
	if (overlapingTargets.Num() == 0) { return; }
	if (!currentTarget)
	{
		float minDistance = 9999.f;
		if (overlapingTargets.Num() == 0)
		{
			return;
		}

		for (int32 i = 0; i < overlapingTargets.Num(); i++)
		{

			if (overlapingTargets.Num() == 0)
			{
				overlapingTargets.Empty();
				break;
			}

			const float distance = (overlapingTargets[i]->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
			if (distance < minDistance)
			{
				minDistance = distance;
				currentTarget = overlapingTargets[i];
				if(currentTarget)
				{
					const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), currentTarget->GetActorLocation());
					GetOwner()->SetActorRotation(lookRotation);
					UUnitComponent* targetComponent = Cast<UUnitComponent>(currentTarget->GetComponentByClass(unitComponentClass));
					if (targetComponent) targetComponent->SetIsTargeted(true);

					GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Red,
					                                 FString::Printf(TEXT("%s is current target of : %s"),
						                                 *currentTarget->GetActorNameOrLabel(),
						                                 *GetOwner()->GetActorNameOrLabel()));
					break;
				}
			}
		}
	}
}

void UUnitComponent::UpdateOverlapingTargets(AActor* target)
{
	if (!target) { return; }

	overlapingTargets.AddUnique(target);

	if (currentTarget) { return;; }

	if(unitType != ETypeUnit::ETU_Miner)
	{
		CheckForClosestTarget();
	}
	else
	{
		currentTarget = target;
	}
	

}

#pragma endregion Combat

#pragma region Movement

void UUnitComponent::NewMove(FVector movePosition)
{
	if (!enemyAIController) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("There is not ai controller for new units"));
		return;
	}
	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Red, FString::Printf(TEXT("Move function")));

	//Move to the ai to given position
	FAIMoveRequest moveRequest;
	moveRequest.SetAcceptanceRadius(40.f);
	bMoveComplete = false;
	//disable dedect collider so when going to its destination and when it counters an enemy doesnt stop so the enemy doesnt attack
	if(ownerUnit)ownerUnit->SetTargetDedectionCollider(ECollisionEnabled::NoCollision);

	if (unitState == UnitStates::EUS_Attacking) 
	{ 
		moveRequest.SetGoalLocation(GetOwner()->GetActorLocation());
		enemyAIController->MoveTo(moveRequest);
		return; 
	}

	moveRequest.SetGoalLocation(movePosition);
	enemyAIController->MoveTo(moveRequest);
	destination = moveRequest.GetDestination();
}

#pragma endregion Movement

#pragma region UnitSelection

void UUnitComponent::SetIsUnitSelected(bool bIsSelected)
{
	if (!bIsUnitSelectable)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not selectable unit"), *GetOwner()->GetActorNameOrLabel());
		return;
	}
	bool selected = false;
	
	if (bIsSelected)
	{
		selected = true;
	}
	else
	{
		
		selected = false;
	}
	
	bIsUnitSelected = selected;
	ownersDecalComponent->SetVisibility(selected);
}

void UUnitComponent::SetOverlayMaterialEmmision(float emmisionAmount)
{
	if (dynamicMaterialInstance)
	{
		const FName	paramName = "EmissiveForce";
		dynamicMaterialInstance->SetScalarParameterValue(paramName, emmisionAmount);
	}
}

#pragma endregion UnitSelection

