// Fill out your copyright notice in the Description page of Project Settings.


#include "CorpseMembers.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
// Sets default values
ACorpseMembers::ACorpseMembers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CutZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CutZone"));

}

// Called when the game starts or when spawned
void ACorpseMembers::BeginPlay()
{
	Super::BeginPlay();
	MemberMesh->
	
}

// Called every frame
void ACorpseMembers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

