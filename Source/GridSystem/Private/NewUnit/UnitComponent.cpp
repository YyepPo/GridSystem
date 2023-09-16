#include "NewUnit/UnitComponent.h"

#include "Components/DecalComponent.h"

#include "Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "AIController.h"
#include "AIController/UnitAIController.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

UUnitComponent::UUnitComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnitComponent::PlayAMontage(UAnimMontage* montage)
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
	//When unit reaches its destination,rotate the unit towards its destination
	if (!GetOwner()) { return; }

	FRotator lookAtTarget = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), destination);
	//Update only Rotation.Yaw
	lookAtTarget.Roll = lookAtTarget.Pitch = 0;
	GetOwner()->SetActorRotation(lookAtTarget);

}

void UUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AttackBehaviour();
}

#pragma region Combat

bool UUnitComponent::IsUnitInAttackRangeOf(AActor* target)
{
	if (!target || !GetOwner()) { return false; }
	//Calculate the distance between target and Owner
	float distanceBtwTarget = (target->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	//Squaring a distance can be computationally faster than calculating a non-squared distance
	float squareRootDistance = FMath::Square(distanceBtwTarget);
	return squareRootDistance <= attackDistance * attackDistance;
}

bool UUnitComponent::CanUnitAttack(AActor* target)
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
		Attack();
	}
	else
	{
		NewMove(currentTarget->GetActorLocation() + currentTarget->GetActorForwardVector() * attackOffset);
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
	if (!currentTarget) { return; }

	//Deal damage to current target
	UGameplayStatics::ApplyDamage(currentTarget,
		attackDamage,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass());

	//If target is dead,set the currentTarget to nullptr, so the unit doesn't keep attacking the dead target
	UUnitComponent* targetsUnitComponent = Cast<UUnitComponent>(currentTarget->GetComponentByClass(unitComponentClass));
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
	PlayAMontage(hitMontage);
	healthAmount -= damage;
	if (healthAmount <= 0)
	{
		bDeadUnit = true;
		OnUnitDeath();
	}
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
		for (int32 i = 0; i < overlapingTargets.Num(); i++)
		{
			float distance = (overlapingTargets[i]->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
			if (distance < minDistance)
			{
				minDistance = distance;
				currentTarget = overlapingTargets[i];
				FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), currentTarget->GetActorLocation());
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
	moveRequest.SetGoalLocation(movePosition);
	moveRequest.SetAcceptanceRadius(40.f);
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

