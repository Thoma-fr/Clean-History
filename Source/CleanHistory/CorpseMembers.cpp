// Fill out your copyright notice in the Description page of Project Settings.


#include "CorpseMembers.h"

#include "Actor/BloodManager.h"
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
	BleedPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BleedingPoint"));
	BleedPoint->SetupAttachment(MemberMesh);
	CutZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CutZone"));
	CutZone->SetupAttachment(MemberMesh, SocketName);

}

// Called when the game starts or when spawned
void ACorpseMembers::BeginPlay()
{
	Super::BeginPlay();
	MemberMesh->GetSocketLocation(SocketName);
	CutZone->AttachToComponent(MemberMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	

	
	if(!GetParentComponent())
		return;

	CutZone->OnComponentBeginOverlap.AddDynamic(this, &ACorpseMembers::OverlapBegin);
	TArray<USceneComponent*> parents;

	GetParentComponent()->GetParentComponents(parents);
	
	if(parents.IsEmpty())
		return;
	if(!Cast<UChildActorComponent>(parents[0]))
		return;

	UChildActorComponent* test = Cast<UChildActorComponent>(parents[0]);
	ACorpseMembers* test2 = Cast<ACorpseMembers>(test->GetChildActor());
	ParentSkelethalMesh = test2->MemberMesh;

	if (ParentSkelethalMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("The parent value is:"));
		return;
	}

	MemberMesh->SetLeaderPoseComponent(ParentSkelethalMesh);
	MemberMesh->AttachToComponent(ParentSkelethalMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MemberMesh->SetSimulatePhysics(true);
	
}

// Called every frame
void ACorpseMembers::Tick(float DeltaTime)
{
	
	CutZone->SetWorldLocation(MemberMesh->GetSocketLocation(SocketName));
	if(hasDetached)
	{
		MemberMesh->SetWorldLocation(lastPos);
		MemberMesh->SetSimulatePhysics(true);
		hasDetached = false;
		if (bloodManager != nullptr)
		{
			myBloodManager = GetWorld()->SpawnActor<ABloodManager>(bloodManager);
			myBloodManager->AttachToComponent(BleedPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

}
void ACorpseMembers::OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UIWeapon::StaticClass()))
	{
		//if(OtherActor->GetVelocity()>)

		MemberLife -= OtherComp->GetComponentVelocity().Length() * 0.20f;

		if(MemberLife>0)
			return;
		MemberMesh->SetLeaderPoseComponent(nullptr);
		TArray<USceneComponent*> parents;
		GetParentComponent()->GetParentComponents(parents);

		if (parents.IsEmpty())
			return;
		if (!Cast<UChildActorComponent>(parents[0]))
			return;

		lastPos = MemberMesh->GetComponentToWorld().GetLocation();
		MemberMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		MemberMesh->SetSimulatePhysics(false);
		hasDetached = true;



		CutZone->OnComponentBeginOverlap.RemoveDynamic(this, &ACorpseMembers::OverlapBegin);
		if (GetParentComponent()->GetChildComponent(0))
		{
			
		}
	}
}

void ACorpseMembers::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
