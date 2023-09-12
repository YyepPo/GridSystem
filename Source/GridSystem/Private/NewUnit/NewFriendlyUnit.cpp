#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Components/SphereComponent.h"

ANewFriendlyUnit::ANewFriendlyUnit():
	unitComponent {CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component")))},
	targetDedectCollider {CreateDefaultSubobject<USphereComponent>(FName(TEXT("Target Dedect Collider")))}
{
	PrimaryActorTick.bCanEverTick = true;
	targetDedectCollider->SetupAttachment(GetRootComponent());
}

void ANewFriendlyUnit::BeginPlay()
{
	Super::BeginPlay();

	if(GetMesh() && GetMesh()->GetAnimInstance()) unitComponent->SetAnimInstance(GetMesh()->GetAnimInstance());

	targetDedectCollider->OnComponentBeginOverlap.AddDynamic(this, &ANewFriendlyUnit::TargetOverlapBegin);
	targetDedectCollider->OnComponentEndOverlap.AddDynamic(this, &ANewFriendlyUnit::TargetOverlapEnd);
}

void ANewFriendlyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewFriendlyUnit::TargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("EnemyTarget")) { return; }

	unitComponent->SetCurrentTarget(OtherActor);
}

void ANewFriendlyUnit::TargetOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



