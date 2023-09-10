#include "UnitBase.h"
#include "Components/DecalComponent.h"

#include "AIController.h"
#include "AIController/UnitAIController.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Resource/ResourceBase.h"

#include "DrawDebugHelpers.h"


AUnitBase::AUnitBase() :
	DecalComponent{ CreateDefaultSubobject<UDecalComponent>(TEXT("Decal")) }
{
	PrimaryActorTick.bCanEverTick = false;
	//Create a decal component
	DecalComponent->SetupAttachment(GetRootComponent());
	DecalComponent->SetVisibility(false);
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

	if (enemyAIController) 
		enemyAIController->OnMoveCompleteMDelegate.AddDynamic(this, &AUnitBase::OnMoveCompleteDelegateHandler);	
}

void AUnitBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUnitBase::PlayAMontage(UAnimMontage* montage)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && montage)
	{
		animInstance->Montage_Play(montage);
	}
}

void AUnitBase::PlayAMontageWithSection(UAnimMontage* montage, FName sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && montage)
	{
		animInstance->Montage_Play(montage);
		animInstance->Montage_JumpToSection(sectionName, montage);
	}

}

bool AUnitBase::IsInAttackRange(AActor* target)
{
	if (!target) { 
		UE_LOG(LogTemp,Warning,TEXT("Target is not valid fomr is in attack range function"))
		return false; }
	const float distanceBtwTarget = (target->GetActorLocation() - GetActorLocation()).Size();
	return distanceBtwTarget < attackDistance;
}

bool AUnitBase::IsAttacking()
{
	return unitState == EUnitState::EUS_Attacking;
}

void AUnitBase::OnUnitSelected(bool isSelected)
{
	if (!bIsSelectable) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("%s is not selectable unit"), *GetActorNameOrLabel());
		return; 
	}

	//a var to keep track of whether the unit is selected or not.
	bool selected = false;

	if (isSelected)
	{
		selected = true;
		//make the emission overlay material to appear
		SetOverlayMaterialEmmision(10.f);
	}
	else
	{
		selected = false;
		//make the emission overlay material to disappear
		SetOverlayMaterialEmmision(0.f);
	}

	bIsSelected = selected;
	// Set the visibility of the DecalComponent based on the selection status.
	DecalComponent->SetVisibility(selected);
}

void AUnitBase::SetRotationTowardsTarget(AActor* target)
{
	if (!target) { return; }
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
	SetActorRotation(lookRotation);
}

void AUnitBase::SetOverlayMaterialEmmision(float emmisionAmount)
{
	UMaterialInstanceDynamic* materialInstance = GetMesh()->CreateDynamicMaterialInstance(0);
	if (materialInstance)
	{
		const FName	paramName = "EmissiveForce";
		materialInstance->SetScalarParameterValue(paramName, emmisionAmount);
	}
}

void AUnitBase::SetUnitToUnOcuppiedState()
{
	unitState = EUnitState::EUS_UnOcuppied;
}

void AUnitBase::Move(FVector MovePosition)
{
	bHasReachedDestination = false;
	targetPosition = MovePosition;

	enemyAIController = (enemyAIController == nullptr) ? Cast<AUnitAIController>(GetController()) : enemyAIController;
	if (!enemyAIController) { return; }

	navigationAcceptableRadius = 130.f;

	FAIMoveRequest moveRequest;
	moveRequest.SetGoalLocation(MovePosition);
	moveRequest.SetAcceptanceRadius(navigationAcceptableRadius);
	enemyAIController->MoveTo(moveRequest);
}

void AUnitBase::MoveToTarget(AActor* target)
{
	if (!target) { return; }
	Move(target->GetActorLocation());
}

void AUnitBase::MoveToTargetInterface(AActor* target)
{
	MoveToTarget(target);
}

void AUnitBase::Attack()
{
	if (isDead) { return; }

	//play attack montage
	PlayAMontage(attackMontage);
	//set unit state to attacking
	unitState = EUnitState::EUS_Attacking;
}

void AUnitBase::GainResourceOnAttack()
{
	GainResource();
}

void AUnitBase::GainResource()
{
	//this function is used for miners
	//when a miner hits a resource, player gains x amount of that resource
	
	const FVector startPos = GetActorLocation();
	const FVector endPos = startPos + GetActorForwardVector() * 10.f;

	FHitResult hitResult;
	const FCollisionShape shape;
	shape.MakeSphere(attackRadius);
	const bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, startPos, endPos, FQuat::Identity, ECollisionChannel::ECC_Vehicle, shape);
	if (hasHit)
	{
		
	}
}

void AUnitBase::OnMoveCompleteDelegateHandler()
{
	if (bIsEnemyUnit) { return; }

	//UE_LOG(LogTemp, Warning, TEXT("Unit has reached its destination"));
	bHasReachedDestination = true;

	const FVector startPos = GetActorLocation();
	const FVector endPos = startPos + GetActorForwardVector() * 100.f;

	const FVector targetVector = targetPosition - startPos;
	// Calculate the dot product between the starting position and the target vector.
	float dotProduct = FVector::DotProduct(startPos, targetVector);

	// Check if the dot product is greater than 0, indicating that the target position is in front of the actor.
	if (dotProduct > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("In Back"));
		FVector direction = targetPosition - GetActorLocation();
		// Calculate the rotation angle in degrees based on the direction vector.
		float rotationAngle = FMath::RadiansToDegrees(FMath::Atan2(direction.Y, direction.X));
		// Create a new rotation with only the Yaw component changed to match the rotation angle.
		FRotator newRotation = FRotator(0.f, rotationAngle, 0.f);
		// Orienting rotation toward the target.
		SetActorRotation(newRotation);
	}
	else
	{
		// If the dot product is not greater than 0, reset the actor's rotation to zero.
		const FRotator zeroRotation = FRotator(0.f, 0.f, 0.f);
		SetActorRotation(zeroRotation);
	}

	GetWorld()->GetTimerManager().SetTimer(testHandle, this, &AUnitBase::TimerHandleTest, rate);
}

void AUnitBase::TimerHandleTest()
{
	bHasReachedDestination = false;
}
