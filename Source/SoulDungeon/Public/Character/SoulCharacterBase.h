#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulCharacterBase.generated.h"

class UHealthComponent;
class UStaminaComponent;
class UManaComponent;
class UEquipmentComponent;
class UCombatComponent;

/*
* 플레이어블 캐릭터의 베이스 클래스
*/

UCLASS()
class SOULDUNGEON_API ASoulCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	ASoulCharacterBase();

protected:

	virtual void BeginPlay() override;

	// === 컴포넌트들 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UManaComponent* ManaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UEquipmentComponent* EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
	
public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// === 데미지 시스템 ===
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	// === 컴포넌트 Getter ===
	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UStaminaComponent* GetStamiaComponent() const { return StaminaComponent; }

	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UManaComponent* GetManaComponent() const { return ManaComponent; }

	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

	UFUNCTION(BlueprintPure, Category ="Components")
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; }

	// === 상태 확인 함수 ===
	UFUNCTION(BlueprintPure, Category = "Character State")
	bool IsDead() const;
	
	UFUNCTION(BlueprintPure, Category ="Character State")
	bool IsAlive() const;

	// === 가상 함수 ( 자식 클래스 오버라이드 ) ===

	// 사망 처리
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	void Die(AActor* Killer);
	virtual void Die_Implementation(AActor* Killer);

	// 기본 공격
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	void Attack();
	virtual void Attack_Implementation();

	// 스킬 사용
	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	void UseSkill(int32 SkillIndex);
	virtual void UseSkill_Implementation(int32 SkillIndex);

private:

	//  컴포넌트 초기화
	void InitializeComponents();

	// 컴포넌트 이벤트 바인딩
	void BindComponentEvents();

	// 사망 이벤트 처리
	UFUNCTION()
	void HandleDeath(AActor* Killer);

};
