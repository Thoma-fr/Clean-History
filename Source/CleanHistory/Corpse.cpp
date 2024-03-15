// Fill out your copyright notice in the Description page of Project Settings.


#include "Corpse.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
ACorpse::ACorpse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACorpse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACorpse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

