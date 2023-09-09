#include "StoneMiner.h"

#include "Kismet/GameplayStatics.h"

#include "Resource/ResourceBase.h"

#include "TypeOfResource.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AStoneMiner::AStoneMiner() :
	weaponStaticMesh{CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Weapon static mesh"))) },
	trajectoryVFXComp{ CreateDefaultSubobject<UNiagaraComponent>(FName(TEXT("Trajectory VFX"))) }
{
	PrimaryActorTick.bCanEverTick = true;
	weaponStaticMesh->SetupAttachment(GetRootComponent());
	trajectoryVFXComp->SetupAttachment(GetRootComponent());

	trajectoryVFXComp->SetVisibility(false);

	isDead = false;
}

void AStoneMiner::BeginPlay()
{
	Super::BeginPlay();
	startPosition = GetActorLocation();

	AttachWeaponToHand();

	if (!aiController)
	{
		UE_LOG(LogTemp, Warning, TEXT("bbb"));
	}
}

void AStoneMiner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if unit is not selected or doesnt have a current target then dont show the trajectory vfx
	if(!bIsSelected || !currentTarget) trajectoryVFXComp->SetVisibility(false);
	if (!currentResourceTarget) { return; }
	if (IsInAttackRange(currentResourceTarget) && !IsAttacking())
	{
		Attack();
	}
	else
	{
		MoveToTarget(currentResourceTarget);
	}
}

void AStoneMiner::MoveToTargetInterface(AActor* target)
{
	MoveToTarget(target);
}

void AStoneMiner::MoveToTarget(AActor* target)
{
	UE_LOG(LogTemp, Warning, TEXT("Move to"));
	
	currentResourceTarget = Cast<AResourceBase>(target);

	if (!currentResourceTarget) { return; }

	bCanMoveToStoneResource = unitType == EunitType::EUT_StoneMiner && currentResourceTarget->GetResourceType() == ETypeResource::ETR_Stone;
	bCanMoveToWoodResource = unitType == EunitType::EUT_WoodChoper && currentResourceTarget->GetResourceType() == ETypeResource::ETR_Wood;

	//if (CanResourceBeMined())
	if(CanResourceBeMined())
	{
		UE_LOG(LogTemp, Warning, TEXT("a"));
		Super::MoveToTarget(target);
		//when unit is selected and has its own target then show a vfx(trajectory) to units destination
		if (bIsSelected && currentResourceTarget)
		{
			trajectoryVFXComp->SetVisibility(true);
			const FName vectorParam = TEXT("Target");
			trajectoryVFXComp->SetVectorParameter(vectorParam, currentResourceTarget->GetActorLocation());
		}
	}
}

bool AStoneMiner::CanResourceBeMined()
{
	return bCanMoveToStoneResource == true || bCanMoveToWoodResource == true;
}

void AStoneMiner::AttachWeaponToHand()
{
	const FName leftHandSocket = "Weapon_Socket_R";
	FAttachmentTransformRules attachmentRule(EAttachmentRule::SnapToTarget, true);
	weaponStaticMesh->AttachToComponent(GetMesh(), attachmentRule, leftHandSocket);
}

void AStoneMiner::Attack()
{
	if (CanResourceBeMined())
	{
		Super::Attack();
		unitState = EUnitState::EUS_Attacking;
		//after an attack set this timer so the unit doesnt attack instantly after the first attack
		GetWorld()->GetTimerManager().SetTimer(attacktTimer, this, &AStoneMiner::AttackRateTimer, attackRate);
	}
}

void AStoneMiner::AttackRateTimer()
{
	unitState = EUnitState::EUS_UnOcuppied;
}

void AStoneMiner::GainResource()
{
	Super::GainResource();

	if (!currentResourceTarget) { return; }
	
	//apply damage to the resource, if resource is destroyed then make the currentResourceTarget null
	UGameplayStatics::ApplyDamage(currentResourceTarget, attackDamage, GetController(), this, UDamageType::StaticClass());
	if (currentResourceTarget->GetIsResourceDestroyed()) currentResourceTarget = nullptr;
	
	//when resource is destroyed go back to start position
	if (!currentResourceTarget) StartToRetreat();
}

void AStoneMiner::StartToRetreat()
{
	GetWorld()->GetTimerManager().SetTimer(retreatTimerHandle, this, &AStoneMiner::Retreat, retreatRate);
}

void AStoneMiner::Retreat()
{
	Move(startPosition);
}
