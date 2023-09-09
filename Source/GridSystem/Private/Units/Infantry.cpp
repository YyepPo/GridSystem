#include "Units/Infantry.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
AInfantry::AInfantry()
{

}

void AInfantry::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;

	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AInfantry::OnSphereOverlapBegin);
}

void AInfantry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AInfantry::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	return DamageAmount;
}

void AInfantry::MoveToTarget(AActor* target)
{

}

void AInfantry::Attack()
{
	Super::Attack();
}

void AInfantry::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AInfantry::RemoveTargetFromOverlapingActors(AInfantry* infatryTarget)
{
}
