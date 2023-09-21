#include "Buildings/MotherBuilding.h"

#include "Components/HealthComponent.h"
#include "NewUnit/UnitComponent.h"

#include "HUD/HealthBarWidget.h"

AMotherBuilding::AMotherBuilding() :
	healthComponent {CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Component")))},
	unitComponent {CreateDefaultSubobject<UUnitComponent>(FName(TEXT("Unit Component")))}
{

}

float AMotherBuilding::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	healthComponent->TakeDamage(DamageAmount);
	healthBarWidget->UpdateHealthBarPercentage(healthComponent->GetHealthPercentage());
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMotherBuilding::OnHit(float damageAmount)
{

}

bool AMotherBuilding::OnDeath()
{
	if (healthComponent->HasDied())
	{
		//play a camera shake

		//destroy object
		OnMotherBuildingDestroyedSignature.Broadcast();
		return true;
	}
	return false;
}


