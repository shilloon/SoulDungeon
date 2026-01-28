#include "Equipments/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{

	PrimaryActorTick.bCanEverTick = false;

	// 메시 컴포넌트 생성
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// 기본 스탯 설정
	BaseDamage = 10.0f;
	AttackSpeed = 1.0f;
	AttackRange = 100.0f;

	// 콜리전 설정 
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("WeaponBase [%s] spawned (Placeholder)"), *GetName());

}

void AWeaponBase::OnEquip()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] Weapon %s equipped"), *GetName());

}

void AWeaponBase::OnUnequip()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] Weapon %s unequipped"), *GetName());

}


