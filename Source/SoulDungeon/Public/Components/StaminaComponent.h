#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaChanged, float, CurrentStamina, float, MaxStamina, float, StaminaPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepleted); // 스태미나 고갈
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaRecovered); // 스태미나 회복

/*
* 캐릭터의 스태미나를 관리하는 컴포넌트
* - 스태미나 소모 / 회복
* - 자동 회복 시스템
* - 스태미나 고갈 시 행동 제한
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDUNGEON_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UStaminaComponent();

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// === 스태미나 속성 ===

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina", meta = (ClampMin = "0.0"))
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	// 초당 자동 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.0"))
	float StaminaRegenRate;

	// 스태미나 사용 후 회복 시작까지 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.0"))
	float RegenDelayAfterUse;

	// 현재 회복 대기 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Stamina")
	float CurrentRegenDelay;

	//스태미나 고갈 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	bool bIsStaminaDepleted;

	// 스태미나 고갈 시 회복 시작 임계값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RecoveryThreshold;

public:	

	// === 델리게이트 ===
	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaDepleted OnStaminaDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaRecovered OnStaminaRecovered;
	
	// === 주요 함수 ===

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float ConsumeStamina(float Amount);

	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool CanConsumeStamina(float Amount) const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float RecoverStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void FullRecover();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetAutoRegenEnabled(bool bEnabled);

	// === Getter 함수 ===

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetStaminaPercentage() const { return MaxStamina > 0.0f ? CurrentStamina / MaxStamina : 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE bool IsStaminaDepleted() const { return bIsStaminaDepleted; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE bool IsFullStamina() const { return CurrentStamina >= MaxStamina; }

	// === Setter 함수 ===

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetMaxStamina(float NewMaxStamina);

private:

	bool bAutoRegenEnabled;

	// 자동 스태미나 회복 처리
	void HandleAutoRegen(float DeltaTime);

	// 스태미나 변경 시 델리게이트 브로드캐스트
	void BroadcastStaminaChanged();

	// 스태미나 고갈 상태 업데이트
	void UpdateDepletedState();

};
