#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

// 마나 변경 시 브로드캐스트할 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnManaChanged, float, CurrentMana, float, MaxMana, float, ManaPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnManaDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnManaRecovered);

/*
*  캐릭터의 마나를 관리하는 컴포넌트
* - 마나 소모 / 회복
* - 자동 회복 시스템
* - 스킬 사용을 위한 마나 관리
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDUNGEON_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UManaComponent();

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===  마나 속성 ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana", meta = (ClampMin = "0.0"))
	float MaxMana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	float CurrentMana;

	// 초당 자동 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (ClampMin = "0.0"))
	float ManaRegenRate;

	// 마나 사용 후 회복 시작까지 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (ClampMin = "0.0"))
	float RegenDelayAfterUse;

	// 현재 회복 대기 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	float CurrentRegenDelay;

	// 마나 고갈 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	bool bIsManaDepleted;

	// 마나 고갈 시 회복 시작 임계값 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (ClampMin = "0.0", ClampMAX = "1.0"))
	float RecoveryThreshold;

public:	

	// === 델리게이트 ===
	UPROPERTY(BlueprintAssignable, Category = "Mana")
	FOnManaChanged OnManaChanged;
		
	UPROPERTY(BlueprintAssignable, Category = "Mana")
	FOnManaDepleted OnManaDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Mana")
	FOnManaRecovered OnManaRecovered;

	// === 주요 함수 ===
	UFUNCTION(BlueprintCallable, Category = "Mana")
	float ConsumeMana(float Amount);

	UFUNCTION(BlueprintPure, Category = "Mana")
	bool CanConsumeMana(float Amount) const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	float RecoverMana(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void FullRecover();

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetAutoRegenEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void ResetRegenDelay();

	// === Getter ===
	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetCurrentMana() const { return CurrentMana; }

	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetMaxMana() const { return MaxMana; }

	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetManaPercentage() const { return MaxMana > 0.0f ? CurrentMana / MaxMana : 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE bool IsManaDepleted() const { return bIsManaDepleted; }

	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE bool IsFullMana() const { return CurrentMana >= MaxMana; }

	// === Setter ===
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetMaxMana(float NewMaxMana);

private:

	bool bAutoRegenEnabled;

	void HandleAutoRegen(float DeltaTime);
	void BroadcastManaChanged();
	void UpdateDepletedState();

};
