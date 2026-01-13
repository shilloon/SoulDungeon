#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulBaseCharacter.generated.h"

UCLASS()
class SOULDUNGEON_API ASoulBaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	ASoulBaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
