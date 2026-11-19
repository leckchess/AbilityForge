#include "UI/AbilityForgeHUDWidget.h"

#include "AbilitySystemComponent.h"
#include "Attributes/AbilityForgeAttributeSet.h"
#include "Components/ProgressBar.h"

namespace
{
	TArray<FGameplayAttribute> GetWatchedAttributes()
	{
		return {
			UAbilityForgeAttributeSet::GetHealthAttribute(),
			UAbilityForgeAttributeSet::GetMaxHealthAttribute(),
			UAbilityForgeAttributeSet::GetStaminaAttribute(),
			UAbilityForgeAttributeSet::GetMaxStaminaAttribute()
		};
	}
}

void UAbilityForgeHUDWidget::InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent)
{
	AbilitySystemComponent = InAbilitySystemComponent;
	if (!InAbilitySystemComponent)
	{
		return;
	}

	for (const FGameplayAttribute& Attribute : GetWatchedAttributes())
	{
		InAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
			.AddUObject(this, &UAbilityForgeHUDWidget::OnAttributeChanged);
	}

	// Show the current values now; the delegates only fire on changes.
	Refresh();
}

void UAbilityForgeHUDWidget::NativeDestruct()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		for (const FGameplayAttribute& Attribute : GetWatchedAttributes())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void UAbilityForgeHUDWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	Refresh();
}

void UAbilityForgeHUDWidget::Refresh()
{
	const UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}

	auto Ratio = [ASC](const FGameplayAttribute& Current, const FGameplayAttribute& Max)
	{
		const float MaxValue = ASC->GetNumericAttribute(Max);
		return MaxValue > 0.f ? ASC->GetNumericAttribute(Current) / MaxValue : 0.f;
	};

	if (HealthBar)
	{
		HealthBar->SetPercent(Ratio(UAbilityForgeAttributeSet::GetHealthAttribute(), UAbilityForgeAttributeSet::GetMaxHealthAttribute()));
	}

	if (StaminaBar)
	{
		StaminaBar->SetPercent(Ratio(UAbilityForgeAttributeSet::GetStaminaAttribute(), UAbilityForgeAttributeSet::GetMaxStaminaAttribute()));
	}
}
