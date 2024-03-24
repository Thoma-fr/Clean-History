// Fill out your copyright notice in the Description page of Project Settings.


#include "CorpseMembers.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Interfaces/IWeapon.h"
// Sets default values
ACorpseMembers::ACorpseMembers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MemberMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MemberMesh"));
	RootComponent = MemberMesh;
	CutZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CutZone"));
	CutZone->AttachToComponent(MemberMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called when the game starts or when spawned
void ACorpseMembers::BeginPlay()
{
	Super::BeginPlay();
	//printFString("My Variable Vector is: %s", *MyVector.ToString());
	TArray<USceneComponent*> chibre;
	GetParentComponent()->GetParentComponents(chibre);
	ParentSkelethalMesh = Cast<USkeletalMeshComponent>(chibre[0]);
	
	//ParentSkelethalMesh = Cast<USkeletalMeshComponent>(Getpa);

	if (!ParentSkelethalMesh)
	{
		//UE_LOG(LogTemp, Warning, TEXT("The parent value is: %s"), GetParentComponent()->GetName());
		return;
	}
	
	MemberMesh->SetLeaderPoseComponent(ParentSkelethalMesh);
	MemberMesh->AttachToComponent(ParentSkelethalMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MemberMesh->SetSimulatePhysics(true);
	CutZone->OnComponentBeginOverlap.AddDynamic(this, &ACorpseMembers::OverlapBegin);
}

// Called every frame
void ACorpseMembers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACorpseMembers::OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IIWeapon* s = Cast<IIWeapon>(OtherActor);
	if (s)
	{
		MemberMesh->SetLeaderPoseComponent(MemberMesh);
		MemberMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}
