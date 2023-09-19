#include "Buildings/MotherBuilding.h"

#include "Components/HealthComponent.h"
#include "HUD/HealthBarWidget.h"

AMotherBuilding::AMotherBuilding() :
	healthComponent {CreateDefaultSubobject<UHealthComponent>(FName(TEXT("Health Component")))}
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
	UE_LOG(LogTemp, Warning, TEXT("Warning Mother base is being attacked"));
}

bool AMotherBuilding::OnDeath()
{
	if (healthComponent->HasDied())
	{
		UE_LOG(LogTemp, Warning, TEXT("Mother base has been destroyed"));
		return true;
	}
	return false;
}


