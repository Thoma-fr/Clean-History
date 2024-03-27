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
	

}

// Called when the game starts or when spawned
void ACorpseMembers::BeginPlay()
{
	Super::BeginPlay();
	MemberMesh->GetSocketLocation(SocketName);
	
	CutZone->AttachToComponent(MemberMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	CutZone->SetupAttachment(MemberMesh, SocketName);

	//printFString("My Variable Vector is: %s", *MyVector.ToString());
	TArray<USceneComponent*> chibre;

	GetParentComponent()->GetParentComponents(chibre);
	/*if (sizeof(chibre) == NULL)
	{
		return;
	}*/
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("My zone Location is: %s"), *CutZone->GetComponentTransform().GetLocation().ToString()));
	ParentSkelethalMesh = Cast<USkeletalMeshComponent>(chibre[0]);
	if (ParentSkelethalMesh == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("The parent value is: %s"), GetParentComponent()->GetName());
		return;
	}
	//ParentSkelethalMesh = Cast<USkeletalMeshComponent>(Getpa);


	
	MemberMesh->SetLeaderPoseComponent(ParentSkelethalMesh);
	MemberMesh->AttachToComponent(ParentSkelethalMesh,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MemberMesh->SetSimulatePhysics(true);
	CutZone->OnComponentBeginOverlap.AddDynamic(this, &ACorpseMembers::OverlapBegin);

	/*MemberMesh->SetLeaderPoseComponent(nullptr);
	MemberMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);*/
}

// Called every frame
void ACorpseMembers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My socket location is: %s"), *MemberMesh->GetSocketLocation(SocketName).ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("My zone Location is: %s"), *CutZone->GetComponentTransform().GetLocation().ToString()));
	CutZone->SetWorldLocation(MemberMesh->GetSocketLocation(SocketName));
	
	
}
void ACorpseMembers::OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//IIWeapon* s = Cast<IIWeapon>(OtherActor);
	if (OtherActor->GetClass()->ImplementsInterface(UIWeapon::StaticClass()))
	{
		MemberMesh->SetLeaderPoseComponent(nullptr);
		MemberMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		if (bloodManager != nullptr)
		{
			myBloodManager = GetWorld()->SpawnActor<ABloodManager>(bloodManager);
			myBloodManager->AttachToComponent(BleedPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		//AttachToComponent(BleedPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CutZone->OnComponentBeginOverlap.RemoveDynamic(this, &ACorpseMembers::OverlapBegin);
	}
}
