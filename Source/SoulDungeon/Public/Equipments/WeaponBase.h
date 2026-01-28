#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

/*
* 모든 무기의 베이스 클래스
*/

UCLASS()
class SOULDUNGEON_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeaponBase();

protected:

	virtual void BeginPlay() override;

	// === 메시 컴포넌트 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	// === 기본 스탯 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float AttackRange;

public:	

	// === Getter 함수 ===
	UFUNCTION(BlueprintPure, Category = "Weapon")
	FORCEINLINE float GetBaseDamage() const { return BaseDamage; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FORCEINLINE float GetAttackRange() const { return AttackRange; }

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	// === 기본 함수 ===
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnEquip();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnUnequip();

};
