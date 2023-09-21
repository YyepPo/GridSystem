#include "NewUnit/UnitComponent.h"

#include "Components/DecalComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "AIController.h"
#include "AIController/UnitAIController.h"

#include "Projectile/Projectile.h"

#include "HitInterface.h"

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
	unitComponentClass = UUnitComponent::StaticClass();

	if (enemyAIController)
			enemyAIController->OnMoveCompletedDelegate.AddDynamic(this, &UUnitComponent::OnMoveCompleteDelegateHandler);
}

void UUnitComponent::OnMoveCompleteDelegateHandler()
{

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

bool UUnitComponent::CanUnitAttack(AActor* target) const
{
	const bool canAttack = target != nullptr &&
		IsUnitInAttackRangeOf(target) &&
		unitState != UnitStates::EUS_Attacking;

	return canAttack;
}

void UUnitComponent::AttackBehaviour()
{
	if (unitState == UnitStates::EUS_Dead || bDeadUnit) { return; }

	if (!currentTarget) { return; }

	if(CanUnitAttack(currentTarget))
	{
		PrintToScreen(FString::Printf(TEXT("Unit can attack mother building")));
		Attack();
		enemyAIController->ShouldDisableMoveRequest(true);
	}
	else
	{
		PrintToScreen(FString::Printf(TEXT("Unit CANNOT attack mother building")));
		NewMove(currentTarget->GetActorLocation());
	}
}

void UUnitComponent::Attack()
{
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

	if (unitType == ETypeUnit::ETU_NormalInfantry || unitType == ETypeUnit::ETU_Cavalry)
	{
		IHitInterface* hitInterface = Cast<IHitInterface>(currentTarget);
		if(hitInterface)
		{
			hitInterface->OnHit(attackDamage);
			if(hitInterface->OnDeath())
			{
				overlapingTargets.Remove(currentTarget);
				currentTarget = nullptr;
				return;
				
			}
			
		}
		//Deal damage to current target
		UGameplayStatics::ApplyDamage(currentTarget,
			attackDamage,
			GetOwner()->GetInstigatorController(),
			GetOwner(),
			UDamageType::StaticClass());


	}
	else if (unitType == ETypeUnit::ETU_Archer && projectileClass && projectileSpawnPositionComp)
	{
		AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, projectileSpawnPositionComp->GetComponentLocation(), projectileSpawnPositionComp->GetComponentRotation());
		if (spawnedProjectile)
		{
			spawnedProjectile->SetOwner(GetOwner());
			spawnedProjectile->SetInstigator(Cast<APawn>(GetOwner()));
			spawnedProjectile->SetProjectileDamage(projectileDamage);
		}
	}

	//If target is dead,set the currentTarget to nullptr, so the unit doesn't keep attacking the dead target
	const UUnitComponent* targetsUnitComponent = Cast<UUnitComponent>(currentTarget->GetComponentByClass(unitComponentClass));
	if (targetsUnitComponent && targetsUnitComponent->GetUnitDead())
	{
		overlapingTargets.Remove(currentTarget);
		targetsUnitComponent = nullptr;
		currentTarget = nullptr;

		CheckForClosestTarget();
	}
}

void UUnitComponent::OnUnitHit(float damage)
{
	if (healthAmount <= 0)
	{
		bDeadUnit = true;
		OnUnitDeath();
		return;
	}
	PlayAMontage(hitMontage);
	healthAmount -= damage;
}

void UUnitComponent::OnUnitDeath()
{
	unitState = UnitStates::EUS_Dead;
	PrintToLog("Unit is dead");
}

void UUnitComponent::OnHit(float damageAmount)
{
	if (OnDeath()) {return;}

	PlayAMontage(hitMontage);
	healthAmount -= damageAmount;
}

bool UUnitComponent::OnDeath()
{
	if (healthAmount <= 0)
	{
		bDeadUnit = true;
		OnUnitDeath();
		return true;
	}
	return false;
}

void UUnitComponent::CheckForClosestTarget()
{
	if (overlapingTargets.Num() == 0) { return; }
	if (!currentTarget)
	{
		float minDistance = 9999.f;
		for (int32 i = 0; i < overlapingTargets.Num(); i++)
		{
			const float distance = (overlapingTargets[i]->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
			if (distance < minDistance)
			{
				minDistance = distance;
				currentTarget = overlapingTargets[i];
				const FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), currentTarget->GetActorLocation());
				GetOwner()->SetActorRotation(lookRotation);
			}
		}
	}
}

void UUnitComponent::UpdateOverlapingTargets(AActor* target)
{
	if (!target) { return; }
	overlapingTargets.AddUnique(target);
	if (overlapingTargets.Num() == 0) { return; }
	if(!currentTarget)currentTarget = overlapingTargets[0];
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

	//Move to the ai to given position
	FAIMoveRequest moveRequest;
	moveRequest.SetAcceptanceRadius(40.f);
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

