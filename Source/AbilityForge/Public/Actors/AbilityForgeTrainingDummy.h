#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilityForgeTrainingDummy.generated.h"

class UAbilitySystemComponent;
class UAbilityForgeAttributeSet;
class UTextRenderComponent;
struct FOnAttributeChangeData;

/**
 * A target to hit. Unlike the player, it owns its own ASC (no PlayerState),
 * shows its health above its head, and heals back to full after reaching 0.
 */
UCLASS()
class ABILITYFORGE_API AAbilityForgeTrainingDummy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAbilityForgeTrainingDummy();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilityForgeAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dummy")
	TObjectPtr<UTextRenderComponent> HealthText;

	/** Seconds at 0 health before the dummy heals back to full. */
	UPROPERTY(EditAnywhere, Category = "Dummy")
	float ResetDelay = 2.f;

private:
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void UpdateHealthText();
	void ResetHealth();

	FTimerHandle ResetTimerHandle;
};
