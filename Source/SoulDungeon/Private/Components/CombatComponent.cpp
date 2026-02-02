#include "Components/CombatComponent.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	// 초기화
	CurrentTarget = nullptr;
	ComboCount = 0;
	bIsBlocking = false;

}

void UCombatComponent::BeginPlay()
{

	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("%s CombatComponent initialized (Placeholder)"), *GetOwner()->GetName());
	
}

void UCombatComponent::PerformAttack()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s performed attack"), *GetOwner()->GetName());

}

void UCombatComponent::PerformSkillAttack(const FString& SkillName)
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s used skill %s"),
		*GetOwner()->GetName(), *SkillName);

}

void UCombatComponent::StartBlocking()
{

	bIsBlocking = true;
	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s started blocking"), *GetOwner()->GetName());

}

void UCombatComponent::StopBlocking()
{

	bIsBlocking = false;
	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s stopped blocking"), *GetOwner()->GetName());

}

void UCombatComponent::PerformDodge()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s performed dodge"), *GetOwner()->GetName());

}

void UCombatComponent::SetTarget(AActor* NewTarget)
{

	CurrentTarget = NewTarget;

	if (NewTarget) {

		UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s set target to: %s"),
			*GetOwner()->GetName(), *NewTarget->GetName());

	}
	else {

		UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s cleared target"), *GetOwner()->GetName());

	}

}

void UCombatComponent::StartCombo()
{

	ComboCount = 1;
	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] %s started combo (Count: %d)"),
		*GetOwner()->GetName(), ComboCount);

}

void UCombatComponent::EndCombo()
{

	UE_LOG(LogTemp, Log, TEXT("[PLCAEHOLDER] %s ended combo (Final Count: %d)"),
		*GetOwner()->GetName(), ComboCount);
	ComboCount = 0;

}