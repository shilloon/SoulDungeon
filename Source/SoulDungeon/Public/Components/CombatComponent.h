#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

/*
*  전투 로직 관리하는 컴포넌트
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDUNGEON_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();

protected:

	virtual void BeginPlay() override;

public:	

	// === 임시 로그 함수들 TODO: 전투 로직 구현하기 ===

	// 기본 공격
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttack();

	// 스킬 공격
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformSkillAttack(const FString& SkillName);

	// 방어 시작
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartBlocking();

	// 방어 종료
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopBlocking();

	// 회피
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformDodge();

	// 타겟 설정
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetTarget(AActor* NewTarget);

	// 콤보 시작
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartCombo();

	// 콤보 종료
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndCombo();

private:

	AActor* CurrentTarget;
	int32 ComboCount;
	bool bIsBlocking;
		
};
