#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth, float, HealthPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, Killer);

/*
++ 캐릭터의 체력을 관리하는 컴포넌트
- 체력 증감
- 사망 처리
- 체력 회복
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDUNGEON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UHealthComponent();

protected:
	
	virtual void BeginPlay() override;

	// === 체력 속성 ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	// 사망 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead;

public:	

	// === 델리게이트 ===
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	// === 주요 함수 ===

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageAmount, AActor* DamageInstigator);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void FullHeal();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Kill(AActor* Killer);

	// === Getter 함수 ===
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealthPercentage() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE bool IsFullHealth() const { return CurrentHealth >= MaxHealth; }

	// === Setter 함수 ===
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float NewMaxHealth);
	
private:

	// 사망 처리 내부 함수
	void HandleDeath(AActor* Killer);
	
	// 체력 변경 델리게이트 브로드캐스트
	void BroadcastHealthChanged();

};
