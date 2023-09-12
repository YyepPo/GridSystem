#include "NewUnit/NewEnemyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Components/SphereComponent.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

ANewEnemyUnit::ANewEnemyUnit() :
	unitComponent{ CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component"))) },
	targetDedectCollider{ CreateDefaultSubobject<USphereComponent>(FName(TEXT("Target Dedect Collider"))) }
{
	PrimaryActorTick.bCanEverTick = true;
	targetDedectCollider->SetupAttachment(GetRootComponent());
}

void ANewEnemyUnit::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh() && GetMesh()->GetAnimInstance()) unitComponent->SetAnimInstance(GetMesh()->GetAnimInstance());

	targetDedectCollider->OnComponentBeginOverlap.AddDynamic(this, &ANewEnemyUnit::TargetOverlapBegin);
	targetDedectCollider->OnComponentEndOverlap.AddDynamic(this, &ANewEnemyUnit::TargetOverlapEnd);
}

void ANewEnemyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewEnemyUnit::TargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("FriendlyTarget")) { 
		PrintToLog("other actor doesnt have FriendlyTarget tag");
		return; }

	unitComponent->SetCurrentTarget(OtherActor);
}

void ANewEnemyUnit::TargetOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


