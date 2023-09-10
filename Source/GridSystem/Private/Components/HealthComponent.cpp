#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = maxHealth;
	heathPercentage = currentHealth / maxHealth;
}

void UHealthComponent::TakeDamage(float damageAmount)
{
	currentHealth = FMath::Max(currentHealth - damageAmount, 0.f);
	heathPercentage = currentHealth / maxHealth;
	if (currentHealth == 0)	OnTowerDestroyedDelegate.Broadcast();
}
