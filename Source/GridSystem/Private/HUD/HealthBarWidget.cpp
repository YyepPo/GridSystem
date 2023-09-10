#include "HUD/HealthBarWidget.h"

#include "Components/ProgressBar.h"

void UHealthBarWidget::UpdateHealthBarPercentage(float healthPercentage)
{
	if (healthBar) healthBar->SetPercent(healthPercentage);
}
