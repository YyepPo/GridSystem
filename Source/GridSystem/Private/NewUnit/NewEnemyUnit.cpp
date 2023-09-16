#include "NewUnit/NewEnemyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Components/HealthComponent.h"

#include "Components/SphereComponent.h"

ANewEnemyUnit::ANewEnemyUnit() :
	healthComponent{ CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Component"))) },
	targetDedectCollider{ CreateDefaultSubobject<USphereComponent>(FName(TEXT("Target Dedect Collider"))) }
{
	PrimaryActorTick.bCanEverTick = true;
	unitComponent = CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component")));
	targetDedectCollider->SetupAttachment(GetRootComponent());
}

void ANewEnemyUnit::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh() && GetMesh()->GetAnimInstance() && unitComponent) unitComponent->SetAnimInstance(GetMesh()->GetAnimInstance());

	targetDedectCollider->OnComponentBeginOverlap.AddDynamic(this, &ANewEnemyUnit::TargetOverlapBegin);
	targetDedectCollider->OnComponentEndOverlap.AddDynamic(this, &ANewEnemyUnit::TargetOverlapEnd);
}

float ANewEnemyUnit::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageAmount == 0 || !healthComponent || !unitComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s deals zero damage , so the unit doesnt take damage"), *DamageCauser->GetActorNameOrLabel());
		return 0;
	}

	unitComponent->OnUnitHit(DamageAmount);
	if (healthComponent->bIsUnitDead)
	{
		unitComponent->OnUnitDeath();
	}

	return DamageAmount;
}

void ANewEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewEnemyUnit::TargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("FriendlyTarget")) { 
		//UE_LOG(LogTemp, Warning, TEXT("overlaping actor :%s doesnt have the FriendlyTarget tag "), *OtherActor->GetActorNameOrLabel());
		return; }
	if (!unitComponent) { return; }
	unitComponent->UpdateOverlapingTargets(OtherActor);
}

void ANewEnemyUnit::TargetOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


