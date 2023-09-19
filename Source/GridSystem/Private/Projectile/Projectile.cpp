#include "Projectile/Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HitInterface.h"

AProjectile::AProjectile() :
	staticMesh {CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"))},
	boxCollider {CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"))},
	projectileMovementComp {CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"))}
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(boxCollider);
	staticMesh->SetupAttachment(GetRootComponent());
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBoxBeginOverlap);
}

void AProjectile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor);
	if (hitInterface) hitInterface->OnHit(damageAmount);

	DealDamage(OtherActor);
	Destroy();
}

void AProjectile::DealDamage(AActor* damagedActor)
{	
	if(damagedActor) UGameplayStatics::ApplyDamage(damagedActor, damageAmount, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
}

