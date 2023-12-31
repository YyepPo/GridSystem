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
	if (bIsDead) return;

	if (currentHealth == 0)
	{
		bIsDead = true;
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
		OnTowerDestroyedDelegate.Broadcast();
		return;
	}
	//Prevents so the currentHealth amount doesn't go below zero
	currentHealth = FMath::Max(currentHealth - damageAmount, 0.f);
	heathPercentage = currentHealth / maxHealth;
	UE_LOG(LogTemp, Warning, TEXT("Damged"));
}
