#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false; // 체력은 Tick 함수가 필요가 없다

	// 기본값
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;

}

void UHealthComponent::BeginPlay()
{

	Super::BeginPlay();

	// 시작 시 체력을 최대치로 설정
	CurrentHealth = MaxHealth;
	bIsDead = false;

	// 초기 체력 상태 브로드캐스트
	BroadcastHealthChanged();

}

float UHealthComponent::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	
	// 이미 사망했거나 데미지가 0 이하이면 무시
	if (bIsDead || DamageAmount <= 0.0f) {

		return 0.0f;

	}

	// 실제 받을 데미지
	const float ActualDamage = FMath::Min(DamageAmount, CurrentHealth);

	// 체력 감소
	CurrentHealth -= ActualDamage;
	CurrentHealth = FMath::Max(CurrentHealth, 0.0f);

	// 체력 변경 이벤트
	BroadcastHealthChanged();

	UE_LOG(LogTemp, Warning, TEXT("%s took %.2f damage. Current Health: %.2f/%.2f"),
		*GetOwner()->GetName(), ActualDamage, CurrentHealth, MaxHealth);
	
	// 체력이 0일 경우 사망 처리
	if (CurrentHealth <= 0.0f) {

		HandleDeath(DamageInstigator);

	}

	return ActualDamage;

}

float UHealthComponent::Heal(float HealAmount)
{

	// 사망했거나 이미 최대 체력일 시 
	if (bIsDead || HealAmount <= 0.0f || IsFullHealth()) {

		return 0.0f;

	}

	// 실제 회복량
	const float ActualHeal = FMath::Min(HealAmount, MaxHealth - CurrentHealth);

	// 체력 증가
	CurrentHealth += ActualHeal;
	CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);

	// 체력 변경 이벤트
	BroadcastHealthChanged();

	UE_LOG(LogTemp, Log, TEXT("%s healed %.2f. Current Health: %.2f/%.2f"),
		*GetOwner()->GetName(), ActualHeal, CurrentHealth, MaxHealth);
	
	return ActualHeal;

}

void UHealthComponent::FullHeal()
{

	if (bIsDead) {
		
		return;

	}

	CurrentHealth = MaxHealth;
	BroadcastHealthChanged();

	UE_LOG(LogTemp, Log, TEXT("%s fully healed to %.2f"), *GetOwner()->GetName(), MaxHealth);

}

void UHealthComponent::Kill(AActor* Killer)
{

	if (bIsDead) {

		return;

	}

	CurrentHealth = 0.0f;
	HandleDeath(Killer);

}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{

	if (NewMaxHealth <= 0.0f) {

		UE_LOG(LogTemp, Warning, TEXT("Attemped to set MaxHealth to invalid value: %.2f"), NewMaxHealth);
		return;

	}

	// 현재 체력 비율 유지
	const float HealthPercentage = GetHealthPercentage();

	MaxHealth = NewMaxHealth;
	CurrentHealth = MaxHealth * HealthPercentage;

	BroadcastHealthChanged();

	UE_LOG(LogTemp, Log, TEXT("%s MaxHealth changed to %.2f. Current Health: %.2f"),
		*GetOwner()->GetName(), MaxHealth, CurrentHealth);

}

void UHealthComponent::HandleDeath(AActor* Killer)
{

	if (bIsDead) {

		return;

	}

	bIsDead = true;
	CurrentHealth = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("%s has died"), *GetOwner()->GetName());

	// 체력 변경 이벤트
	BroadcastHealthChanged();

	// 사망 이벤트 브로드캐스트
	OnDeath.Broadcast(Killer);

}

void UHealthComponent::BroadcastHealthChanged()
{

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, GetHealthPercentage());

}
