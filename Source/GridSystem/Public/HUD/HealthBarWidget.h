
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

UCLASS()
class GRIDSYSTEM_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable)
		void UpdateHealthBarPercentage(float healthPercentage);

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* healthBar;
};
