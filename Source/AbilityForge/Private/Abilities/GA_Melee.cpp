#include "Abilities/GA_Melee.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Core/AbilityForgeGameplayTags.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UGA_Melee::UGA_Melee()
{
	FGameplayTagContainer MeleeAssetTags;
	MeleeAssetTags.AddTag(AbilityForgeTags::Ability_Melee);
	SetAssetTags(MeleeAssetTags);

	ActivationOwnedTags.AddTag(AbilityForgeTags::State_Attacking);

	ActivationBlockedTags.AddTag(AbilityForgeTags::State_Dashing);
	ActivationBlockedTags.AddTag(AbilityForgeTags::State_Stunned);
}

void UGA_Melee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!AttackMontage || !CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	HitActors.Reset();

	// Listen for the impact frame first, so an early notify is never missed.
	UAbilityTask_WaitGameplayEvent* WaitHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, AbilityForgeTags::Event_Melee_Hit);
	WaitHit->EventReceived.AddDynamic(this, &UGA_Melee::OnHitEvent);
	WaitHit->ReadyForActivation();

	// Tasks end with the ability, and the montage stops if the ability ends early.
	UAbilityTask_PlayMontageAndWait* PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AttackMontage);
	PlayMontage->OnCompleted.AddDynamic(this, &UGA_Melee::OnMontageFinished);
	PlayMontage->OnBlendOut.AddDynamic(this, &UGA_Melee::OnMontageFinished);
	PlayMontage->OnInterrupted.AddDynamic(this, &UGA_Melee::OnMontageCancelled);
	PlayMontage->OnCancelled.AddDynamic(this, &UGA_Melee::OnMontageCancelled);
	PlayMontage->ReadyForActivation();
}

void UGA_Melee::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Melee::OnMontageCancelled()
{
	// Interrupted by another montage or cancelled: end cleanly so State.Attacking is removed.
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Melee::OnHitEvent(FGameplayEventData Payload)
{
	// The montage plays on client and server, but only the server decides what was hit.
	if (HasAuthority(&CurrentActivationInfo))
	{
		TraceForHits();
	}
}

void UGA_Melee::TraceForHits()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UWorld* World = GetWorld();
	if (!Avatar || !World)
	{
		return;
	}

	const FVector Start = Avatar->GetActorLocation();
	const FVector End = Start + Avatar->GetActorForwardVector() * TraceDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(MeleeTrace), false, Avatar);
	TArray<FHitResult> Hits;
	World->SweepMultiByObjectType(
		Hits, Start, End, FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(TraceRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	if (bDrawDebugTrace)
	{
		DrawDebugCapsule(World, (Start + End) * 0.5f, TraceDistance * 0.5f + TraceRadius, TraceRadius,
			FRotationMatrix::MakeFromZ(End - Start).ToQuat(), Hits.Num() > 0 ? FColor::Green : FColor::Red, false, 1.f);
	}
#endif

	const float DamageAmount = Damage.GetValueAtLevel(GetAbilityLevel());

	for (const FHitResult& Hit : Hits)
	{
		AActor* Target = Hit.GetActor();

		// A character can be hit on its capsule and its mesh; count it once.
		if (!Target || HitActors.Contains(Target))
		{
			continue;
		}
		HitActors.Add(Target);

		if (ApplyDamageToTarget(Target, DamageAmount))
		{
		}
	}
}
