#include "Components/ManaComponent.h"

UManaComponent::UManaComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// 기본값 설정
	MaxMana = 100.0f;
	CurrentMana = MaxMana;
	ManaRegenRate = 5.0f;
	RegenDelayAfterUse = 3.0f;
	RecoveryThreshold = 0.2f;

	CurrentRegenDelay = 0.0f;
	bIsManaDepleted = false;
	bAutoRegenEnabled = true;

}

void UManaComponent::BeginPlay()
{

	Super::BeginPlay();
	
	// 시작 시 마나 최대치 설정
	CurrentMana = MaxMana;
	bIsManaDepleted = false;
	CurrentRegenDelay = 0.0f;

	BroadcastManaChanged();

}

void UManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAutoRegenEnabled) {

		HandleAutoRegen(DeltaTime);

	}

}

float UManaComponent::ConsumeMana(float Amount) 
{

	if (Amount < 0.0f) {

		return 0.0f;

	}

	const float ActualConsumption = FMath::Min(Amount, CurrentMana);

	// 마나 감소
	CurrentMana -= ActualConsumption;
	CurrentMana = FMath::Max(CurrentMana, 0.0f);

	// 회복 대기 시간 리셋
	CurrentRegenDelay = RegenDelayAfterUse;

	// 상태 업데이트
	UpdateDepletedState();
	BroadcastManaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s consumed %.2f mana. Current: %.2f/%.2f"),
		*GetOwner()->GetName(), ActualConsumption, CurrentMana, MaxMana);

	return ActualConsumption;

}

bool UManaComponent::CanConsumeMana(float Amount) const 
{

	return CurrentMana >= Amount;

}

float UManaComponent::RecoverMana(float Amount)
{

	if (Amount <= 0.0f || IsFullMana()) {

		return 0.0f;

	}

	const float ActualRecovery = FMath::Min(Amount, MaxMana - CurrentMana);

	// 마나 증가
	CurrentMana += ActualRecovery;
	CurrentMana = FMath::Min(CurrentMana, MaxMana);

	// 상태 업데이트
	UpdateDepletedState();
	BroadcastManaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s recovered %.2f mana. Current: %.2f/%.2f"),
		*GetOwner()->GetName(), ActualRecovery, CurrentMana, MaxMana);

	return ActualRecovery;

}

void UManaComponent::FullRecover()
{

	CurrentMana = MaxMana;
	CurrentRegenDelay = 0.0f;

	UpdateDepletedState();
	BroadcastManaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s mana fully recovered to %.2f"),
		*GetOwner()->GetName(), MaxMana);

}

void UManaComponent::SetAutoRegenEnabled(bool bEnabled) 
{

	bAutoRegenEnabled = bEnabled;

	if (!bEnabled) {

		CurrentRegenDelay = 0.0f;

	}

	UE_LOG(LogTemp, Log, TEXT("%s auto mana regen %s"),
		*GetOwner()->GetName(), bEnabled ? TEXT("enabled") : TEXT("disabled"));

}

void UManaComponent::ResetRegenDelay()
{

	CurrentRegenDelay = 0.0f;
	UE_LOG(LogTemp, Log, TEXT("%s mana regen delay reset"), *GetOwner()->GetName());

}

void UManaComponent::SetMaxMana(float NewMaxMana) 
{

	if (NewMaxMana <= 0.0f) {

		UE_LOG(LogTemp, Warning, TEXT("Attemped to set MaxMana to invalid value: %.2f"), NewMaxMana);
		return;

	}

	// 현재 마나 비율 유지
	const float ManaPercentage = GetManaPercentage();

	MaxMana = NewMaxMana;
	CurrentMana = MaxMana * ManaPercentage;

	BroadcastManaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s MaxMana changed to %.2f. Current: %.2f"),
		*GetOwner()->GetName(), MaxMana, CurrentMana);

}

void UManaComponent::HandleAutoRegen(float DeltaTime)
{

	// 이미 최대치 시 회복 불필요
	if (IsFullMana()) {

		CurrentRegenDelay = 0.0f;
		return;

	}

	// 회복 대기 시간 처리
	if (CurrentRegenDelay > 0.0f) {

		CurrentRegenDelay -= DeltaTime;
		return;

	}

	// 마나 자동 회복
	const float RegenAmount = ManaRegenRate * DeltaTime;
	RecoverMana(RegenAmount);

}

void UManaComponent::BroadcastManaChanged()
{

	OnManaChanged.Broadcast(CurrentMana, MaxMana, GetManaPercentage());

}

void UManaComponent::UpdateDepletedState()
{

	const bool bWasDepleted = bIsManaDepleted;

	// 마나가 0이면 고갈 상태
	if (CurrentMana <= 0.0f) {

		bIsManaDepleted = true;

	}

	// 임계값 이상 회복 시 고갈 상태 해제
	else if (bIsManaDepleted && GetManaPercentage() >= RecoveryThreshold) {

		bIsManaDepleted = false;

	}

	// 상태 변화 시 델리게이트 브로드캐스트
	if (bWasDepleted != bIsManaDepleted) {

		if (bIsManaDepleted) {

			UE_LOG(LogTemp, Warning, TEXT("%s mana depleted!"), *GetOwner()->GetName());
			OnManaDepleted.Broadcast();

		}
		else {

			UE_LOG(LogTemp, Log, TEXT("%s mana recovered!"), *GetOwner()->GetName());
			OnManaRecovered.Broadcast();

		}
	}

}