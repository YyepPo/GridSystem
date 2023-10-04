#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Components/HealthComponent.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

#include "NewUnit/TypeUnit.h"

ANewFriendlyUnit::ANewFriendlyUnit():
	healthComponent {CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Component")))},
	targetDedectCollider {CreateDefaultSubobject<USphereComponent>(FName(TEXT("Target Dedect Collider")))},
	selectionDecalComponent {CreateDefaultSubobject<UDecalComponent>(FName(TEXT("Selection Decal Component")))},
	minersWeaponStaticMesh {CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Miners Weapon Static Mesh")))}
{
	PrimaryActorTick.bCanEverTick = true;

	targetDedectCollider->SetupAttachment(GetRootComponent());
	minersWeaponStaticMesh->SetupAttachment(GetRootComponent());
	selectionDecalComponent->SetupAttachment(GetRootComponent());

	unitComponent = CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component")));
}

void ANewFriendlyUnit::BeginPlay()
{
	Super::BeginPlay();

	targetDedectCollider->OnComponentBeginOverlap.AddDynamic(this, &ANewFriendlyUnit::TargetOverlapBegin);

	AttachWeaponToMinersHand();

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
	if (!OtherActor->ActorHasTag(targetTag)) return; 
	if (!unitComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s doesnt have a unitComponent"),*GetActorNameOrLabel());
		return;
	}
	unitComponent->UpdateOverlapingTargets(OtherActor);
}


void ANewFriendlyUnit::AttachWeaponToMinersHand() const
{
	if (!unitComponent )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s doesnt have a unitComponent"),*GetActorNameOrLabel());
		return;
	}

	if(unitComponent->GetUnitsType() != ETypeUnit::ETU_Miner )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is not a resource miner,dont attach a weapon,there is already a weapon"),*GetActorNameOrLabel());
		return;
	}

	if(minersWeaponStaticMesh->GetStaticMesh() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s miner doesnt have a weapon static mesh"),*GetActorNameOrLabel());
		return;
	}

	const FName leftHandSocket = "Weapon_Socket_R";
	FAttachmentTransformRules attachmentRule(EAttachmentRule::SnapToTarget, true);
	minersWeaponStaticMesh->AttachToComponent(GetMesh(), attachmentRule, leftHandSocket);
}
