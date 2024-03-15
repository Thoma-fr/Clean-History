// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CleanHistory/Interfaces/IWeapon.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class CLEANHISTORY_API AWeapon : public AActor, public IIWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	uint32 Damage;
	/*UPROPERTY(EditAnywhere)
	float MinVelocity, VelocityMultipliyer;*/

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;


};
