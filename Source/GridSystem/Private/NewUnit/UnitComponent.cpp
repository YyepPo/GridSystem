#include "NewUnit/UnitComponent.h"

#include "AIController.h"
#include "AIController/UnitAIController.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

UUnitComponent::UUnitComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnitComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UUnitComponent::PlayAMontage(UAnimMontage* montage)
{
	if (!animInstance || !montage) { return; }
	animInstance->Montage_Play(montage);
}

void UUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!currentTarget) { return; }


	if (CanUnitAttack(currentTarget))
	{
		Attack();
	}
	else
	{
		NewMove(currentTarget->GetActorLocation());
	}

}

#pragma region Movement

void UUnitComponent::NewMove(FVector movePosition)
{
	if (!enemyAIController) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("There is not ai controller for new units"));
		return;
	}
		
	FAIMoveRequest moveRequest;
	moveRequest.SetGoalLocation(movePosition);
	moveRequest.SetAcceptanceRadius(40.f);
	enemyAIController->MoveTo(moveRequest);
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

	bIsSelected = selected;
	bIsUnitSelected = bIsSelected;
	if (bIsSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is selected"), *GetOwner()->GetActorNameOrLabel());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is NOT selected"), *GetOwner()->GetActorNameOrLabel());
	}
}

#pragma endregion UnitSelection

#pragma region Combat

bool UUnitComponent::IsUnitInAttackRangeOf(AActor* target)
{
	if (!target || !GetOwner()) { return false; }
	float distanceBtwTarget = (target->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	float squareRootDistance = FMath::Square(distanceBtwTarget);
	return squareRootDistance <= attackDistance * attackDistance;
}

bool UUnitComponent::CanUnitAttack(AActor* target)
{
	const bool canAttack = target != nullptr && 
		IsUnitInAttackRangeOf(target) && 
		unitState != EUnitsState::EUS_Attacking;

	return canAttack;
}

void UUnitComponent::Attack()
{
	PlayAMontage(attackMontage);
	unitState = EUnitsState::EUS_Attacking;
	GetWorld()->GetTimerManager().SetTimer(attackRateTimerHandle, this,&UUnitComponent::AttackRateHandle, attackRate);
}

void UUnitComponent::AttackRateHandle()
{
	unitState = EUnitsState::EUS_UnOcuppied;
}

#pragma endregion Combat