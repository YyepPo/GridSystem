#include "NewUnit/NewFriendlyUnit.h"
#include "NewUnit/UnitComponent.h"

#include "Components/HealthComponent.h"

#include "Components/SphereComponent.h"

#define PrintToLog(message) UE_LOG(LogTemp,Warning,TEXT(message));

ANewFriendlyUnit::ANewFriendlyUnit():
	unitComponent {CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component")))},
	healthComponent {CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Component")))},
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

float ANewFriendlyUnit::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageAmount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s deals zero damage , so the unit doesnt take damage"), *DamageCauser->GetActorNameOrLabel());
		return 0;
	}

	if (healthComponent->HasUnitDied()) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("%s has died"), *GetOwner()->GetActorNameOrLabel());
		return 0; 
	}

	healthComponent->TakeDamage(DamageAmount);

	return DamageAmount;
}

void ANewFriendlyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewFriendlyUnit::TargetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("EnemyTarget")) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("overlaping actor :%s doesnt have the EnemyTarget tag "), *OtherActor->GetActorNameOrLabel());
		return; 
	}

	unitComponent->UpdateOverlapingTargets(OtherActor);
}

void ANewFriendlyUnit::TargetOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



