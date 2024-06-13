// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AProp::AProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProp::ApplyDamageOverTime(uint32 DamagePerSecond)
{
    if (Health > 0)
    {
        Damage = DamagePerSecond;

        GetWorldTimerManager().SetTimer(DamageTimer, this, &AProp::DealDamagePerSecond, 1.0f, true, 1.0f);
    }
    else {
        Die();
    }
}

void AProp::DealDamagePerSecond()
{
    Health -= Damage;

    if (Health <= 0)
    {
        GetWorldTimerManager().ClearTimer(DamageTimer);
        Die();
    }
}

void AProp::Die() 
{
    if (BurnOnDestroySound != nullptr)
        UGameplayStatics::PlaySoundAtLocation(this, BurnOnDestroySound, GetActorLocation());

    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

