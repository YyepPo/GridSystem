#include "HUD/HealthBarWidget.h"

#include "Components/ProgressBar.h"

void UHealthBarWidget::UpdateHealthBarPercentage(float healthPercentage) const
{
	if (healthBar) healthBar->SetPercent(healthPercentage);
}
