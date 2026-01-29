#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShieldBase.generated.h"

/*
*  모든 방패 베이스 클래스
*/

UCLASS()
class SOULDUNGEON_API AShieldBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AShieldBase();

protected:

	virtual void BeginPlay() override;

	// === 메시 컴포넌트 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShieldMesh;

	// === 기본 스탯 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Stats")
	float BlockAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Stats")
	float StaminaCostPerBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield Stats")
	float DurabilityMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield Stats")
	float DurabilityCurrent;

public:	

	// === Getter 함수 ===
	UFUNCTION(BlueprintPure, Category = "Shield")
	FORCEINLINE float GetBlockAmount() const { return BlockAmount; }

	UFUNCTION(BlueprintPure, Category = "Shield")
	FORCEINLINE float GetStaminaCostPerBlock() { return StaminaCostPerBlock; }

	UFUNCTION(BlueprintPure, Category = "Shield")
	FORCEINLINE float GetDurability() const { return DurabilityCurrent; }

	UFUNCTION(BlueprintPure, Category = "Shield")
	FORCEINLINE float GetMaxDurability() const { return DurabilityMax; }

	UFUNCTION(BlueprintPure, Category = "Shield")
	FORCEINLINE UStaticMeshComponent* GetShieldMesh() const { return ShieldMesh; }

	// === 기본 함수 ===
	UFUNCTION(BlueprintCallable, Category = "Shield")
	virtual void OnEquip();

	UFUNCTION(BlueprintCallable, Category = "Shield")
	virtual void OnUnequip();

	UFUNCTION(BlueprintCallable, Category = "Shield")
	virtual void StartBlocking();

	UFUNCTION(BlueprintCallable, Category = "Shield")
	virtual void StopBlocking();


};
