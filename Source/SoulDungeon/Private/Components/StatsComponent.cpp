#include "Components/StatsComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"
#include "Components/StaminaComponent.h"

UStatsComponent::UStatsComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	// 초기 스탯
	VigorValue = 10;
	MindValue = 10;
	EnduranceValue = 10;
	StrengthValue = 10;
	DexterityValue = 10;
	IntelligenceValue = 10;
	FaithValue = 10;
	ArcaneValue = 10;

	// 스탯 제한
	MinStatValue = 10;
	MaxStatValue = 99;

	// 스탯당 증가량
	HealthPerVigor = 30.0f;
	ManaPerMind = 10.0f;
	StaminaPerEndurance = 5.0f;

}

void UStatsComponent::BeginPlay()
{

	Super::BeginPlay();

	// 초기 스탯 기반으로 컴포넌트 업데이트
	UpdateHealthComponent();
	UpdateManaComponent();
	UpdateStaminaComponent();

	UE_LOG(LogTemp, Log, TEXT("%s StatsComponent initialized - Total Level: %d"),
		*GetOwner()->GetName(), GetTotalLevel());
	
}

bool UStatsComponent::IncreaseStat(ECharacterStatType StatType)
{

	return IncreaseStatByAmount(StatType, 1);

}

bool UStatsComponent::IncreaseStatByAmount(ECharacterStatType StatType, int32 Amount)
{

	if (Amount <= 0) {

		return false;

	}

	int32* StatPtr = GetStatPointer(StatType);
	if (!StatPtr) {

		return false;

	}

	// 최대치 확인
	if (*StatPtr >= MaxStatValue) {

		UE_LOG(LogTemp, Warning, TEXT("Stat already at maximun (%d)"), MaxStatValue);
		return false;

	}

	// 증가
	int32 OldValue = *StatPtr;
	*StatPtr = FMath::Clamp(*StatPtr + Amount, MinStatValue, MaxStatValue);
	int32 ActualIncrease = *StatPtr - OldValue;

	if (ActualIncrease > 0) {

		UE_LOG(LogTemp, Log, TEXT("Stat increased: %d -> %d (+%d)"),
			OldValue, *StatPtr, ActualIncrease);
	
		UpdateRelatedComponents(StatType);

		OnStatChanged.Broadcast(StatType, *StatPtr);

		return true;

	}

	return false;

}

void UStatsComponent::SetStat(ECharacterStatType StatType, int32 NewValue)
{

	int32* StatPtr = GetStatPointer(StatType);

	if (!StatPtr) {

		return;

	}

	NewValue = FMath::Clamp(NewValue, MinStatValue, MaxStatValue);
	if (*StatPtr != NewValue) {

		int32 OldValue = *StatPtr;
		*StatPtr = NewValue;

		UE_LOG(LogTemp, Log, TEXT("Stat set: %d -> %d"), OldValue, NewValue);

		UpdateRelatedComponents(StatType);

		OnStatChanged.Broadcast(StatType, NewValue);

	}

}

int32 UStatsComponent::GetStat(ECharacterStatType StatType) const
{

	switch (StatType) {

	case ECharacterStatType::ST_Vigor:
		return VigorValue;

	case ECharacterStatType::ST_Mind:
		return MindValue;

	case ECharacterStatType::ST_Endurance:
		return EnduranceValue;

	case ECharacterStatType::ST_Strength:
		return StrengthValue;

	case ECharacterStatType::ST_Dexterity:
		return DexterityValue;

	case ECharacterStatType::ST_Intelligence:
		return IntelligenceValue;

	case ECharacterStatType::ST_Faith:
		return FaithValue;

	case ECharacterStatType::ST_Arcane:
		return ArcaneValue;

	default:
		return 0;

	}

}

bool UStatsComponent::IsStatMaxed(ECharacterStatType StatType) const
{

	return GetStat(StatType) >= MaxStatValue;

}

bool UStatsComponent::MeetsRequirements(int32 ReqStr, int32 ReqDex, int32 ReqInt, int32 ReqFaith, int32 ReqArcane) const
{

	return StrengthValue >= ReqStr &&
		DexterityValue >= ReqDex &&
		IntelligenceValue >= ReqInt &&
		FaithValue >= ReqFaith &&
		ArcaneValue >= ReqArcane;

}

float UStatsComponent::CalculateMaxHealth() const
{

	const float BaseHealth = 100.0f;
	return BaseHealth + (VigorValue * HealthPerVigor);

}

float UStatsComponent::CalculateMaxMana() const
{

	const float BaseMana = 50.0f;
	return BaseMana + (MindValue * ManaPerMind);

}

float UStatsComponent::CalculateMaxStamina() const
{

	const float BaseStamina = 80.0f;
	return BaseStamina + (EnduranceValue * StaminaPerEndurance);

}

int32 UStatsComponent::GetTotalLevel() const
{

	// 총 레벨 = 모든 스탯의 합 - 초기화(80)
	const int32 InitialStatTotal = MinStatValue * 8;
	const int32 CurrentStatTotal = VigorValue + MindValue + EnduranceValue + StrengthValue + DexterityValue + IntelligenceValue + FaithValue + ArcaneValue;

	return CurrentStatTotal - InitialStatTotal;

}

int32* UStatsComponent::GetStatPointer(ECharacterStatType StatType)
{

	switch (StatType) {

	case ECharacterStatType::ST_Vigor:
		return &VigorValue;

	case ECharacterStatType::ST_Mind:
		return &MindValue;

	case ECharacterStatType::ST_Endurance:
		return &EnduranceValue;

	case ECharacterStatType::ST_Strength:
		return &StrengthValue;

	case ECharacterStatType::ST_Dexterity:
		return &DexterityValue;

	case ECharacterStatType::ST_Intelligence:
		return &IntelligenceValue;

	case ECharacterStatType::ST_Faith:
		return &FaithValue;

	case ECharacterStatType::ST_Arcane:
		return &ArcaneValue;

	default:
		return nullptr;

	}

}

void UStatsComponent::UpdateRelatedComponents(ECharacterStatType StatType)
{

	switch (StatType) {

	case ECharacterStatType::ST_Vigor:
		UpdateHealthComponent();
		break;

	case ECharacterStatType::ST_Mind:
		UpdateManaComponent();
		break;

	case ECharacterStatType::ST_Endurance:
		UpdateStaminaComponent();
		break;

	}

}

void UStatsComponent::UpdateHealthComponent()
{

	UHealthComponent* HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (HealthComp) {

		const float NewMaxHealth = CalculateMaxHealth();
		HealthComp->SetMaxHealth(NewMaxHealth);

		UE_LOG(LogTemp, Log, TEXT("MaxHealth updated to %.2f (Vigor: %d)"),
			NewMaxHealth, VigorValue);

	}

}

void UStatsComponent::UpdateManaComponent()
{

	UManaComponent* ManaComp = GetOwner()->FindComponentByClass<UManaComponent>();
	if (ManaComp) {

		const float NewMaxMana = CalculateMaxMana();
		ManaComp->SetMaxMana(NewMaxMana);

		UE_LOG(LogTemp, Log, TEXT("MaxMana updated to %.2f (Mind: %d)"),
			NewMaxMana, MindValue);

	}

}

void UStatsComponent::UpdateStaminaComponent()
{

	UStaminaComponent* StaminaComp = GetOwner()->FindComponentByClass<UStaminaComponent>();
	if (StaminaComp) {

		const float NewMaxStamina = CalculateMaxStamina();
		StaminaComp->SetMaxStamina(NewMaxStamina);

		UE_LOG(LogTemp, Log, TEXT("MaxStamina updated to %.2f (Endurance: %d)"),
			NewMaxStamina, EnduranceValue);

	}

}
