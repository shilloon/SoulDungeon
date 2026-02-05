#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

// 스탯 타입 열거형
UENUM(BlueprintType)
enum class ECharacterStatType : uint8
{
	ST_Vigor UMETA(DisplayName = "Vigor"), // 활력 - HP
	ST_Mind UMETA(DisplayName = "Mind"), // 정신력 - MP
	ST_Endurance UMETA(DisplayName = "Endurance"), // 체력 - 스태미나
	ST_Strength UMETA(DisplayName = "Strength"), // 근력 - 물리 데미지
	ST_Dexterity UMETA(DisplayName = "Dexterity"), // 기량 - 공격속도
	ST_Intelligence UMETA(DisplayName = "Intelligence"), // 지력 - 마법
	ST_Faith UMETA(DisplayName = "Faith"), // 신앙 - 기적 / 버프
	ST_Arcane UMETA(DisplayName = "Arcane") // 신비 - 아이템 / 상태이상
};

// 스탯 변경 시 브로드캐스트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, ECharacterStatType, StatType, int32, NewValue);

/*
*  캐릭터 스탯 관리하는 컴포넌트
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDUNGEON_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UStatsComponent();

protected:

	virtual void BeginPlay() override;

	// === 스탯 값들 ===
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 VigorValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 MindValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 EnduranceValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 StrengthValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 DexterityValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 IntelligenceValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 FaithValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10", ClampMax = "99"))
	int32 ArcaneValue;

	// === 스탯 제한 ===
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats|Limits")
	int32 MinStatValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Limits")
	int32 MaxStatValue;

	// === 스탯당 증가량 ===

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats|Scaling")
	float HealthPerVigor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats|Scaling")
	float ManaPerMind;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats|Scaling")
	float StaminaPerEndurance;

public:	

	// === 델리 게이트 ===
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnStatChanged;
	
	// === 스탯 증가 함수 ===

	// 특정 스탯 1증가
	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IncreaseStat(ECharacterStatType StatType);

	// 특정 스탯 특정 값만큼 증가
	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IncreaseStatByAmount(ECharacterStatType StatType, int32 Amount);

	// 특정 스탯 특정 값으로 설정
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetStat(ECharacterStatType StatType, int32 NewValue);

	// === Getter ===
	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetStat(ECharacterStatType StatType) const;

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetVigor() const { return VigorValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetMind() const { return MindValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetEndurance() const { return EnduranceValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetStrength() const { return StrengthValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetDexterity() const { return DexterityValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetIntelligence() const { return IntelligenceValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetFaith() const { return FaithValue; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE int32 GetArcane() const { return ArcaneValue; }

	// === 스탯 체크 함수 ===

	// 특정 스탯 최대치 확인
	UFUNCTION(BlueprintPure, Category = "Stats")
	bool IsStatMaxed(ECharacterStatType StatType) const;

	// 무기 요구 스탯 만족하는지 확인
	UFUNCTION(BlueprintPure, Category = "Stats")
	bool MeetsRequirements(int32 ReqStr, int32 ReqDex, int32 ReqInt, int32 ReqFaith, int32 ReqArcane) const;

	// === 계산 값 함수 ===

	// 현재 스탯 기반 최대 체력 계산
	UFUNCTION(BlueprintPure, Category = "Stats")
	float CalculateMaxHealth() const;

	// 현재 스탯 기반 최대 마나 계산
	UFUNCTION(BlueprintPure, Category = "Stats")
	float CalculateMaxMana() const;

	// 현재 스탯 기반 최대 스태미나 계산
	UFUNCTION(BlueprintPure, Category = "Stats")
	float CalculateMaxStamina() const;

	// 총 레벨 계산 (모든 스탯 합 - 초기값)
	UFUNCTION(BlueprintPure, Category = "Stats")
	int32 GetTotalLevel() const;
	
private:

	// 스탯 포인터 반환
	int32* GetStatPointer(ECharacterStatType StatType);

	// 스탯 변경 시 관련 컴포넌트 업데이트
	void UpdateRelatedComponents(ECharacterStatType StatType);

	// 체력/마나/스태미나 컴포넌트 업데이트
	void UpdateHealthComponent();
	void UpdateManaComponent();
	void UpdateStaminaComponent();

};
