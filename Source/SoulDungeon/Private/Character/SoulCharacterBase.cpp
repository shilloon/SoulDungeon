#include "Character/SoulCharacterBase.h"
#include "Components/HealthComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/ManaComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/CombatComponent.h"

ASoulCharacterBase::ASoulCharacterBase()
{

	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	ManaComponent = CreateDefaultSubobject<UManaComponent>(TEXT("ManaComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

}

void ASoulCharacterBase::BeginPlay()
{

	Super::BeginPlay();
	
	// 컴포넌트 초기화 및 이벤트 바인딩
	InitializeComponents();
	BindComponentEvents();

	UE_LOG(LogTemp, Log, TEXT("CharacterBase [%s] initialized with all components"), *GetName());

}

void ASoulCharacterBase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void ASoulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ASoulCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{

	// 이미 사망 시 데미지 무시
	if (IsDead()) {

		return 0.0f;

	}

	// HealthComponent에 데미지 전달
	if (HealthComponent) {

		const float ActualDamage = HealthComponent->TakeDamage(DamageAmount, DamageCauser);

		UE_LOG(LogTemp, Warning, TEXT("%s took %.2f damage from %s"),
			*GetName(), ActualDamage, DamageCauser ? *DamageCauser->GetName() : TEXT("UnKnown"));
	
		return ActualDamage;

	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

}

bool ASoulCharacterBase::IsDead() const
{

	return HealthComponent ? HealthComponent->IsDead() : false;

}

bool ASoulCharacterBase::IsAlive() const 
{

	return !IsDead();

}

void ASoulCharacterBase::Die_Implementation(AActor* Killer)
{

	// 기본 사망 처리
	UE_LOG(LogTemp, Warning, TEXT("%s has died! (Killed by: %s)"),
		*GetName(), Killer ? *Killer->GetName() : TEXT("UnKnown"));

}

void ASoulCharacterBase::Attack_Implementation()
{

	// 사망 상태시 공격 불가
	if (IsDead()) {

		return;

	}

	// 스태미나 확인
	if (StaminaComponent && !StaminaComponent->CanConsumeStamina(10.0f)) {

		UE_LOG(LogTemp, Warning, TEXT("%s cannot attack - not enough stamina"), *GetName());
		return;

	}

	// 스태미나 소모
	if (StaminaComponent) {

		StaminaComponent->ConsumeStamina(10.0f);

	}

	// CombatComponent 호출
	if (CombatComponent) {

		CombatComponent->PerformAttack();

	}

	UE_LOG(LogTemp, Log, TEXT("[BASE] %s performed basic attack"), *GetName());

}

void ASoulCharacterBase::UseSkill_Implementation(int32 SkillIndex)
{

	// 사망 상태 시 스킬 불가
	if (IsDead()) {

		return;

	}

	// 마나 확인
	if (ManaComponent && !ManaComponent->CanConsumeMana(20.0f)) {

		UE_LOG(LogTemp, Warning, TEXT("%s cannot use skill - not enough mana"), *GetName());
		return;

	}

	// 마나 소모
	if (ManaComponent) {

		ManaComponent->ConsumeMana(20.0f);

	}

	// CombatComponent 호출
	if (CombatComponent) {

		CombatComponent->PerformSkillAttack(FString::Printf(TEXT("Skill_%d"), SkillIndex));

	}

	UE_LOG(LogTemp, Log, TEXT("[BASE] %s used skill %d"), *GetName(), SkillIndex);

}

void ASoulCharacterBase::InitializeComponents()
{

	if (!HealthComponent || !StaminaComponent || !ManaComponent ||
		!EquipmentComponent || !CombatComponent) {

		UE_LOG(LogTemp, Error, TEXT("SoulCharacterBase: One or more components failed to initialize!"));
		return;

	}

	UE_LOG(LogTemp, Log, TEXT("All components initialized successfully for %s"), *GetName());

}

void ASoulCharacterBase::BindComponentEvents()
{

	// 사망 이벤트 바인딩
	if (HealthComponent) {

		HealthComponent->OnDeath.AddDynamic(this, &ASoulCharacterBase::HandleDeath);

	}

}

void ASoulCharacterBase::HandleDeath(AActor* Killer)
{

	Die(Killer);

	DisableInput(Cast<APlayerController>(GetController()));

	UE_LOG(LogTemp, Warning, TEXT("%s death handled"), *GetName());

}