#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

// 전방 선언
class AWeaponBase;
class AShieldBase;

// 장비 슬롯 열거형
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	MainHand UMETA(DisplayName = "Main Hand"),
	OffHand UMETA(DisplayName = "off Hand"),
	TwoHanded UMETA(DisplayName = "Two Handed")
};

// 장비 변경 시 브로드캐스트할 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponEquipped, AWeaponBase*, NewWeapon, EEquipmentSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponUnequipped, AWeaponBase*, OldWeapon, EEquipmentSlot, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldEquipped, AShieldBase*, NewShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldUnequipped, AShieldBase*, OldShield);


/*
* 캐릭터의 장비를 관리하는 컴포넌트
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULDUNGEON_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEquipmentComponent();

protected:

	virtual void BeginPlay() override;

	// === 장작된 장비 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	AWeaponBase* MainHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	AWeaponBase* OffHandWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	AWeaponBase* TwoHandedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	AShieldBase* Shield;

	// === 소켓 이름 ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Sockets")
	FName MainHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Sockets")
	FName OffHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Sockets")
	FName TwoHandedSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Sockets")
	FName ShieldSocketName;

public:	

	// === 델리게이트 ===
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnWeaponEquipped OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnWeaponUnequipped OnWeaponUnequipped;

	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnShieldEquipped OnShieldEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnShieldUnequipped OnShieldUnequipped;

	// === 무기 장착/해제 ===
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipWeapon(AWeaponBase* Weapon, EEquipmentSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	AWeaponBase* UnequipWeapon(EEquipmentSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipAllWeapons();

	// === 방패 장착/해제 ===
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipShield(AShieldBase* NewShield);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	AShieldBase* UnequipShield();

	// === Getter 함수 ===
	UFUNCTION(BlueprintPure, Category = "Equipment")
	FORCEINLINE AWeaponBase* GetMainHandWeapon() const { return MainHandWeapon; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	FORCEINLINE AWeaponBase* GetOffHandWeapon() const { return OffHandWeapon; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	FORCEINLINE AWeaponBase* GetTwoHandedWeapon() const { return TwoHandedWeapon; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	FORCEINLINE AShieldBase* GetShield() const { return Shield; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool HasWeaponEquipped(EEquipmentSlot Slot) const;

	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool HasShieldEquipped() const { return Shield != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	bool IsTwoHandedMode() const { return TwoHandedWeapon != nullptr; }

private:

	// 무기를 캐릭터 소켓에 부착
	bool AttachWeaponToSocket(AWeaponBase* Weapon, FName SocketName);

	// 방패를 캐릭터 소켓에 부착
	bool AttachShieldToSocket(AShieldBase* Shield, FName SocketName);

	// 장비 캐릭터에서 분리
	void DetachEquipment(AActor* Equipment);

	// 양손 무기 장착 시 다른 무기/방패 자동 해제
	void HandleTwoHandedEquip();

};
