#include "Equipments/ShieldBase.h"
#include "Components/StaticMeshComponent.h"

AShieldBase::AShieldBase()
{

	PrimaryActorTick.bCanEverTick = false;

	// 메시 컴포넌트 생성
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	RootComponent = ShieldMesh;

	// 기본 스탯 설정
	BlockAmount = 0.5f;
	StaminaCostPerBlock = 5.0f;
	DurabilityMax = 100.0f;
	DurabilityCurrent = DurabilityMax;

	// 콜리전 설정
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AShieldBase::BeginPlay()
{
	Super::BeginPlay();
	
	DurabilityCurrent = DurabilityMax;
	UE_LOG(LogTemp, Log, TEXT("ShiledBase [%s] spawned (Placeholder)"), *GetName());

}

void AShieldBase::OnEquip()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] Shield %s equipped"), *GetName());

}

void AShieldBase::OnUnequip()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] Shield %s unequipped"), *GetName());

}

void AShieldBase::StartBlocking()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] Shield %s started blocking"), *GetName());

}

void AShieldBase::StopBlocking()
{

	UE_LOG(LogTemp, Log, TEXT("[PLACEHOLDER] Shield %s stopped blocking"), *GetName());

}
