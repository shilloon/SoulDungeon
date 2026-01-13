#include "Characters/SoulBaseCharacter.h"

ASoulBaseCharacter::ASoulBaseCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}

void ASoulBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoulBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

