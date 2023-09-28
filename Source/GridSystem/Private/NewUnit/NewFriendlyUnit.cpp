#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Components/HealthComponent.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

ANewFriendlyUnit::ANewFriendlyUnit():
	healthComponent {CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Component")))},
	targetDedectCollider {CreateDefaultSubobject<USphereComponent>(FName(TEXT("Target Dedect Collider")))},
	selectionDecalComponent {CreateDefaultSubobject<UDecalComponent>(FName(TEXT("Selection Decal Component")))}
{
	PrimaryActorTick.bCanEverTick = true;
	unitComponent = CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component")));

	targetDedectCollider->SetupAttachment(GetRootComponent());
	selectionDecalComponent->SetupAttachment(GetRootComponent());
}

void ANewFriendlyUnit::BeginPlay()
{
	Super::BeginPlay();

	targetDedectCollider->OnComponentBeginOverlap.AddDynamic(this, &ANewFriendlyUnit::TargetOverlapBegin);
	targetDedectCollider->OnComponentEndOverlap.AddDynamic(this, &ANewFriendlyUnit::TargetOverlapEnd);

	if(GetMesh() && GetMesh()->GetAnimInstance() && unitComponent) unitComponent->SetAnimInstance(GetMesh()->GetAnimInstance());

	if (unitComponent && GetMesh())
	{
		unitComponent->SetDynamicMaterialInstance(GetMesh()->CreateDynamicMaterialInstance(0));
		unitComponent->SetSelectionDecalComponent(selectionDecalComponent);
	}
}

float ANewFriendlyUnit::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageAmount == 0 || !unitComponent) return 0;
	unitComponent->OnHit(DamageAmount);
	return DamageAmount;
}

void ANewFriendlyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewFriendlyUnit::SetTargetDedectionCollider(ECollisionEnabled::Type collisionType)
{
	targetDedectCollider->SetCollisionEnabled(collisionType);
}

void ANewFriendlyUnit::TargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("EnemyTarget")) return; 
	if (!unitComponent) { return; }
	unitComponent->UpdateOverlapingTargets(OtherActor);
}

void ANewFriendlyUnit::TargetOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
