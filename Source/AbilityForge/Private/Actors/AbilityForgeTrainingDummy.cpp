#include "Actors/AbilityForgeTrainingDummy.h"

#include "AbilitySystemComponent.h"
#include "Attributes/AbilityForgeAttributeSet.h"
#include "Components/TextRenderComponent.h"
#include "TimerManager.h"

AAbilityForgeTrainingDummy::AAbilityForgeTrainingDummy()
{
	// The dummy just stands there; it doesn't need an AI controller.
	AutoPossessAI = EAutoPossessAI::Disabled;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Minimal: no player owns this actor, so nobody needs full Gameplay Effect data.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAbilityForgeAttributeSet>(TEXT("AttributeSet"));

	HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
	HealthText->SetupAttachment(RootComponent);
	HealthText->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthText->SetHorizontalAlignment(EHTA_Center);
	HealthText->SetWorldSize(24.f);
	HealthText->SetTextRenderColor(FColor::Red);
}

UAbilitySystemComponent* AAbilityForgeTrainingDummy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAbilityForgeTrainingDummy::BeginPlay()
{
	Super::BeginPlay();

	// The dummy is both owner and avatar of its ASC.
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAbilityForgeAttributeSet::GetHealthAttribute())
		.AddUObject(this, &AAbilityForgeTrainingDummy::OnHealthChanged);

	UpdateHealthText();
}

void AAbilityForgeTrainingDummy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateHealthText();

	if (HasAuthority() && Data.NewValue <= 0.f && !GetWorldTimerManager().IsTimerActive(ResetTimerHandle))
	{
		GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AAbilityForgeTrainingDummy::ResetHealth, ResetDelay, false);
	}
}

void AAbilityForgeTrainingDummy::UpdateHealthText()
{
	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"),
		AttributeSet->GetHealth(), AttributeSet->GetMaxHealth())));
}

void AAbilityForgeTrainingDummy::ResetHealth()
{
	AbilitySystemComponent->SetNumericAttributeBase(UAbilityForgeAttributeSet::GetHealthAttribute(), AttributeSet->GetMaxHealth());
}
