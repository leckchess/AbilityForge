#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityForgeHUDWidget.generated.h"

class UAbilitySystemComponent;
class UProgressBar;
struct FOnAttributeChangeData;

/**
 * Health and stamina bars driven by attribute change delegates, so nothing ticks or polls.
 * Make a Widget Blueprint from this class with two Progress Bars named HealthBar and StaminaBar.
 */
UCLASS(Abstract)
class ABILITYFORGE_API UAbilityForgeHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent);

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

private:
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	void Refresh();

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
