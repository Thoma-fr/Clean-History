// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/IBurnable.h"
#include "Prop.generated.h"

UCLASS()
class CLEANHISTORY_API AProp : public AActor, public IIBurnable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyDamageOverTime(uint32 DamagePerSecond) override;

	virtual void DealDamagePerSecond() override;

	virtual void Die();

	UPROPERTY(EditAnywhere)
	uint32 Health;

	UPROPERTY()
	uint32 Damage;

	UPROPERTY()
	FTimerHandle DamageTimer;
};
