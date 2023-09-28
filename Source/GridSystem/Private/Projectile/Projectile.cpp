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
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBoxBeginOverlap);
}
 
void AProjectile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() && OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile overllaped with the owner"));
		return;;
	}

	if(!OtherActor->ActorHasTag(targetTag))
	{
		return;
	}

	IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor);
	if (hitInterface) hitInterface->OnHit(damageAmount);

	DealDamage(OtherActor);

	if (objectPoolingActor)
	{
		objectPoolingActor->ReturnPooledObject(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("objectPooling is not valid"));
	}
}

void AProjectile::DealDamage(AActor* damagedActor)
{	
	if (damagedActor && GetOwner())
	{
		UGameplayStatics::ApplyDamage(damagedActor, damageAmount, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: doesnt have an Owner"), *GetActorNameOrLabel());
	}
}

