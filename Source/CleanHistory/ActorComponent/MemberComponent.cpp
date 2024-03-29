// Fill out your copyright notice in the Description page of Project Settings.


#include "MemberComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values for this component's properties
UMemberComponent::UMemberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CutZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CutZone"));
	MemberMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MemberMesh"));
	// ...
}


// Called when the game starts
void UMemberComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ParentSkelethalMesh != nullptr)
		MemberMesh->SetLeaderPoseComponent(ParentSkelethalMesh);
	// ...
	
}


// Called every frame
void UMemberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

