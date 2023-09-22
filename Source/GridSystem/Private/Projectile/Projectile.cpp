#include "Projectile/Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HitInterface.h"
#include "ObjectPooling/ObjectPooling.h"

AProjectile::AProjectile() :
	staticMesh {CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"))},
	boxCollider {CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"))},
	projectileMovementComp {CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"))}
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(boxCollider);
	staticMesh->SetupAttachment(GetRootComponent());
}

void AProjectile::Activate()
{
	bIsActive = true;
	projectileMovementComp->bSimulationEnabled = true;
	projectileMovementComp->InitialSpeed = 800;
	projectileMovementComp->MaxSpeed = 800;

	OnActivate();

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (GetOwner()) SetActorLocation(GetOwner()->GetActorLocation());

}

void AProjectile::Deactivate()
{
	bIsActive = false;
	projectileMovementComp->bSimulationEnabled = false;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (GetOwner()) SetActorLocation(GetOwner()->GetActorLocation());

	SetOwner(nullptr);
	SetInstigator(nullptr);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AProjectile::Return, 5.f);
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBoxBeginOverlap);
}

void AProjectile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) { return;; }

	IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor);
	if (hitInterface) hitInterface->OnHit(damageAmount);

	DealDamage(OtherActor);

	if (objectPoolingActor)
	{
		objectPoolingActor->ReturnPooledObject(this);
	}
	
}

void AProjectile::DealDamage(AActor* damagedActor)
{	
	if(damagedActor) UGameplayStatics::ApplyDamage(damagedActor, damageAmount, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
}

