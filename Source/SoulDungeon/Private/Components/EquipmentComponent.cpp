#include "Components/EquipmentComponent.h"
#include "Equipments/WeaponBase.h"
#include "Equipments/ShieldBase.h"
#include "GameFramework/Character.h"

UEquipmentComponent::UEquipmentComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	// 기본 소켓 이름 설정
	MainHandSocketName = FName("hand_r_socket");
	OffHandSocketName = FName("hand_l_socket");
	TwoHandedSocketName = FName("hand_r_socket");
	ShieldSocketName = FName("hand_l_socket");

	// 초기화
	MainHandWeapon = nullptr;
	OffHandWeapon = nullptr;
	TwoHandedWeapon = nullptr;
	Shield = nullptr;

}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("%s EquipmentComponent initialized"), *GetOwner()->GetName());
	
}

bool UEquipmentComponent::EquipWeapon(AWeaponBase* Weapon, EEquipmentSlot Slot)
{

	if (!Weapon) {

		UE_LOG(LogTemp, Warning, TEXT("Attempted to equip null weapon"));
		return false;
		
	}

	// 기존 무기 해제
	AWeaponBase* OldWeapon = UnequipWeapon(Slot);

	// 양손 무기 장착 시 다른 장비 해제
	if (Slot == EEquipmentSlot::TwoHanded) {

		HandleTwoHandedEquip();

	}

	// 슬롯에 따라 처리
	FName SocketName;
	switch (Slot) {

	case EEquipmentSlot::MainHand:

		if (TwoHandedWeapon) {

			UE_LOG(LogTemp, Warning, TEXT("Cannot equip main hand weapon while two-handed weapon is equipped"));
			return false;

		}

		MainHandWeapon = Weapon;
		SocketName = MainHandSocketName;
		break;

	case EEquipmentSlot::OffHand:

		if (TwoHandedWeapon) {

			UE_LOG(LogTemp, Warning, TEXT("Cannot equip off hand weapon while two-handed weapon is equipped"));
			return false;

		}

		OffHandWeapon = Weapon;
		SocketName = OffHandSocketName;
		break;

	case EEquipmentSlot::TwoHanded:

		TwoHandedWeapon = Weapon;
		SocketName = TwoHandedSocketName;
		break;

	default:
		
		UE_LOG(LogTemp, Error, TEXT("Invalid equipment slot"));
		return false;

	}

	// 무기를 소켓에 부착
	if (AttachWeaponToSocket(Weapon, SocketName)) {

		UE_LOG(LogTemp, Log, TEXT("%s equipped weapon in slot %d"),
			*GetOwner()->GetName(), static_cast<int32>(Slot));

		OnWeaponEquipped.Broadcast(Weapon, Slot);
		return true;

	}

	return false;

}

AWeaponBase* UEquipmentComponent::UnequipWeapon(EEquipmentSlot Slot)
{

	AWeaponBase* WeaponToUnequip = nullptr;

	switch (Slot) {

	case EEquipmentSlot::MainHand:
		WeaponToUnequip = MainHandWeapon;
		MainHandWeapon = nullptr;
		break;

	case EEquipmentSlot::OffHand:
		WeaponToUnequip = OffHandWeapon;
		OffHandWeapon = nullptr;
		break;

	case EEquipmentSlot::TwoHanded:
		WeaponToUnequip = TwoHandedWeapon;
		TwoHandedWeapon = nullptr;
		break;

	}

	if (WeaponToUnequip) {

		DetachEquipment(WeaponToUnequip);

		UE_LOG(LogTemp, Log, TEXT("%s unequipped weapon from slot %d"),
			*GetOwner()->GetName(), static_cast<int32>(Slot));

		OnWeaponUnequipped.Broadcast(WeaponToUnequip, Slot);

	}

	return WeaponToUnequip;

}

void UEquipmentComponent::UnequipAllWeapons()
{

	UnequipWeapon(EEquipmentSlot::MainHand);
	UnequipWeapon(EEquipmentSlot::OffHand);
	UnequipWeapon(EEquipmentSlot::TwoHanded);

	UE_LOG(LogTemp, Log, TEXT("%s unequipped all weapons"), *GetOwner()->GetName());

}


bool UEquipmentComponent::EquipShield(AShieldBase* NewShield)
{

	if (!NewShield) {

		UE_LOG(LogTemp, Warning, TEXT("Attemped to equip null shield"));
		return false;

	}

	// 양손 무기가 있을 시 방패 장착 불가
	if (TwoHandedWeapon) {

		UE_LOG(LogTemp, Warning, TEXT("Cannot equip shield while two-handed weapon is equipped"));
		return false;

	}

	// 기존 방패 해제
	UnequipShield();

	// 방패 소켓에 부착
	if (AttachShieldToSocket(NewShield, ShieldSocketName)) {

		Shield = NewShield;

		UE_LOG(LogTemp, Log, TEXT("%s equipped shield"), *GetOwner()->GetName());

		OnShieldEquipped.Broadcast(NewShield);
		
		return true;

	}

	return false;

}

AShieldBase* UEquipmentComponent::UnequipShield()
{

	if (!Shield) {

		return nullptr;

	}

	AShieldBase* OldShield = Shield;
	Shield = nullptr;

	DetachEquipment(OldShield);

	UE_LOG(LogTemp, Log, TEXT("%s unequipped shield"), *GetOwner()->GetName());

	OnShieldUnequipped.Broadcast(OldShield);

	return OldShield;

}

bool UEquipmentComponent::HasWeaponEquipped(EEquipmentSlot Slot) const
{

	switch(Slot) {

	case EEquipmentSlot::MainHand:
		return MainHandWeapon != nullptr;

	case EEquipmentSlot::OffHand:
		return OffHandWeapon != nullptr;

	case EEquipmentSlot::TwoHanded:
		return TwoHandedWeapon != nullptr;

	default:
		return false;
	}

}

bool UEquipmentComponent::AttachWeaponToSocket(AWeaponBase* Weapon, FName SocketName)
{

	if (!Weapon) {

		return false;

	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) {

		UE_LOG(LogTemp, Error, TEXT("EquipmentComponent owner is not a Character"));
		return false;

	}
	
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) {

		UE_LOG(LogTemp, Error, TEXT("Character has no skeletal mesh"));
		return false;

	}

	// 무기를 소켓에 부착
	Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	Weapon->OnEquip();

	UE_LOG(LogTemp, Log, TEXT("Weapon attached to socket: %s"), *SocketName.ToString());
	
	return true;

}

bool UEquipmentComponent::AttachShieldToSocket(AShieldBase* ShieldToAttach, FName SocketName)
{

	if (!ShieldToAttach) {

		return false;

	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) {

		UE_LOG(LogTemp, Error, TEXT("EquipmentComponent owner is not a Character"));
		return false;

	}

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) {

		UE_LOG(LogTemp, Error, TEXT("Character has no skeletal mesh"));
		return false;

	}

	// 방패 소켓 부착
	ShieldToAttach->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	ShieldToAttach->OnEquip();

	UE_LOG(LogTemp, Log, TEXT("Shield attached to socket: %s"), *SocketName.ToString());
	
	return true;

}

void UEquipmentComponent::DetachEquipment(AActor* Equipment)
{

	if (!Equipment) {

		return;

	}

	// 실제 분리 로직
	Equipment->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 장비 종류별로 OnUnequip 호출
	if (AWeaponBase* Weapon = Cast<AWeaponBase>(Equipment)) {

		Weapon->OnUnequip();

	}

	else if (AShieldBase* ShieldPtr = Cast<AShieldBase>(Equipment))
	{

		ShieldPtr->OnUnequip();

	}

	UE_LOG(LogTemp, Log, TEXT("Equipment detached"));

}

void UEquipmentComponent::HandleTwoHandedEquip()
{

	UnequipWeapon(EEquipmentSlot::MainHand);
	UnequipWeapon(EEquipmentSlot::OffHand);
	UnequipShield();

	UE_LOG(LogTemp, Log, TEXT("Cleared equipment for two-handed weapon"));

}