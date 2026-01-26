#include "Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// 기본값 설정
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaRegenRate = 10.0f;
	RegenDelayAfterUse = 2.0f;
	RecoveryThreshold = 0.3f;

	CurrentRegenDelay = 0.0f;
	bIsStaminaDepleted = false;
	bAutoRegenEnabled = true;

}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 스태미나 최대치 설정
	CurrentStamina = MaxStamina;
	bIsStaminaDepleted = false;
	CurrentRegenDelay = 0.0f;
	
	// 초기 상태 브로드캐스트
	BroadcastStaminaChanged();

}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bAutoRegenEnabled) {

		HandleAutoRegen(DeltaTime);

	}

}

float UStaminaComponent::ConsumeStamina(float Amount)
{
	
	if (Amount <= 0.0f) {

		return 0.0f;

	}

	// 실제 소모량 계산
	const float ActualConsumption = FMath::Min(Amount, CurrentStamina);

	// 스태미나 감소
	CurrentStamina -= ActualConsumption;
	CurrentStamina = FMath::Max(CurrentStamina, 0.0f);

	// 회복 대기 시간 리셋
	CurrentRegenDelay = RegenDelayAfterUse;

	// 상태 업데이트
	UpdateDepletedState();
	BroadcastStaminaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s consumed %.2f stamina. Current: %.2f/%.2f"),
		*GetOwner()->GetName(), ActualConsumption, CurrentStamina, MaxStamina);

	return ActualConsumption;

}

bool UStaminaComponent::CanConsumeStamina(float Amount) const
{

	return CurrentStamina >= Amount;

}

float UStaminaComponent::RecoverStamina(float Amount)
{

	if (Amount <= 0.0f || IsFullStamina()) {

		return 0.0f;

	}

	// 실제 회복량 계산
	const float ActualRecovery = FMath::Min(Amount, MaxStamina - CurrentStamina);

	// 스태미나 회복
	CurrentStamina += ActualRecovery;
	CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);

	// 상태 업데이트
	UpdateDepletedState();
	BroadcastStaminaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s recovered %.2f stamina. Current: %.2f/%.2f"),
		*GetOwner()->GetName(), ActualRecovery, CurrentStamina, MaxStamina);

	return ActualRecovery;

}

void UStaminaComponent::FullRecover()
{

	CurrentStamina = MaxStamina;
	CurrentRegenDelay = 0.0f;

	UpdateDepletedState();
	BroadcastStaminaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s stamina fully recovered to %.2f"),
		*GetOwner()->GetName(), MaxStamina);

}

void UStaminaComponent::SetAutoRegenEnabled(bool bEnabled)
{

	bAutoRegenEnabled = bEnabled;

	if (!bEnabled) {

		CurrentRegenDelay = 0.0f;

	}

	UE_LOG(LogTemp, Log, TEXT("%s auto stamina regen %s"),
		*GetOwner()->GetName(), bEnabled ? TEXT("enabled") : TEXT("disabled"));

}

void UStaminaComponent::SetMaxStamina(float NewMaxStamina)
{

	if (NewMaxStamina <= 0.0f) {

		UE_LOG(LogTemp, Warning, TEXT("Attemped to set MaxStamina to invalid value: %.2f"), NewMaxStamina);
		return;

	}

	// 현재 스태미나 비율 유지
	const float StaminaPercentage = GetStaminaPercentage();

	MaxStamina = NewMaxStamina;
	CurrentStamina = MaxStamina * StaminaPercentage;

	BroadcastStaminaChanged();

	UE_LOG(LogTemp, Log, TEXT("%s MaxStamina changed to %.2f. Current: %.2f"),
		*GetOwner()->GetName(), MaxStamina, CurrentStamina);

}

void UStaminaComponent::HandleAutoRegen(float DeltaTime)
{

	// 이미 최대치는 붎필요
	if (IsFullStamina()) {

		CurrentRegenDelay = 0.0f;
		return;

	}

	// 회복 대기 시간 처리
	if (CurrentRegenDelay > 0.0f) {

		CurrentRegenDelay -= DeltaTime;
		return;

	}

	// 스태미나 자동 회복
	const float RegenAmount = StaminaRegenRate * DeltaTime;
	RecoverStamina(RegenAmount);

}

void UStaminaComponent::BroadcastStaminaChanged()
{

	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, GetStaminaPercentage());

}

void UStaminaComponent::UpdateDepletedState()
{

	const bool bWasDepleted = bIsStaminaDepleted;

	// 스태미나가 0이면 고갈 상태
	if (CurrentStamina <= 0.0f) {

		bIsStaminaDepleted = true;

	}

	// 임계값 이상 회복 시 고갈 상태 해제
	else if (bIsStaminaDepleted && GetStaminaPercentage() >= RecoveryThreshold) {

		bIsStaminaDepleted = false;

	}

	// 상태 변화 시 델리게이트 브로드캐스트
	if (bWasDepleted != bIsStaminaDepleted) {

		if (bIsStaminaDepleted) {

			UE_LOG(LogTemp, Warning, TEXT("%s stamina depleted!"), *GetOwner()->GetName());
			OnStaminaDepleted.Broadcast();

		}
		else {

			UE_LOG(LogTemp, Log, TEXT("%s stamina recovered!"), *GetOwner()->GetName());
			OnStaminaRecovered.Broadcast();

		}
	}

}

