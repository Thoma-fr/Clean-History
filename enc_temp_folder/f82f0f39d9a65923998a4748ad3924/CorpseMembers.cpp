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
	BleedPoint->SetupAttachment(MemberMesh,SocketName);
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
	BleedPoint->SetWorldLocation(MemberMesh->GetSocketLocation(SocketName));
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
	//LineTrace
	//bool ishidedX = false;
	//bool ishidedY = false;
	//bool ishidedZ = false;
	//bool ishidedminusX = false;
	//bool ishidedminusY = false;
	//bool ishidedminusZ = false;

	FHitResult Hitx;
	FHitResult Hity;
	FHitResult Hitz;
	FHitResult Hitminusx;
	FHitResult Hitminusy;
	FHitResult Hitminusz;


	FVector TraceStart = FVector(CutZone->GetComponentToWorld().GetLocation().X, CutZone->GetComponentToWorld().GetLocation().Y-20.f, CutZone->GetComponentToWorld().GetLocation().Z);
	FVector TraceEndx = TraceStart+FVector(1, 0, 0) * 100.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredComponent(MemberMesh);

	bool ishidedX = GetWorld()->LineTraceSingleByChannel(Hitx, TraceStart + FVector(1, 0, 0) * 30.f, TraceEndx,TraceChannelProperty, QueryParams);
	bool ishidedY = GetWorld()->LineTraceSingleByChannel(Hity,  TraceStart + FVector(0, 1, 0) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedZ = GetWorld()->LineTraceSingleByChannel(Hitz,  TraceStart + FVector(0, 0, 1) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedminusX = GetWorld()->LineTraceSingleByChannel(Hitminusx, TraceStart + FVector(-1, 0, 0) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedminusY = GetWorld()->LineTraceSingleByChannel(Hitminusy, TraceStart + FVector(0, -1, 0) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedminusZ = GetWorld()->LineTraceSingleByChannel(Hitminusz, TraceStart + FVector(0, 0, -1) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);


	
	DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(1, 0, 0) * 30.f, Hitx.bBlockingHit ? FColor::Red : FColor::Magenta, false, .1f, 0, 2.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, 1, 0) * 30.f, Hity.bBlockingHit ? FColor::Green : FColor::Magenta, false, .1f, 0, 2.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, 0, 1) * 30.f, Hitz.bBlockingHit ? FColor::Blue : FColor::Magenta, false, .1f, 0, 2.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(-1, 0, 0) * 30.f, Hitminusx.bBlockingHit ? FColor::Red : FColor::Magenta, false, .1f, 0, 2.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, -1, 0) * 30.f, Hitminusy.bBlockingHit ? FColor::Green : FColor::Magenta, false, .1f, 0, 2.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, 0, -1) * 30.f, Hitminusz.bBlockingHit ? FColor::Blue : FColor::Magenta, false, .1f, 0, 2.0f);
	int count = ishidedX + ishidedY + ishidedZ + ishidedminusX + ishidedminusY + ishidedminusZ;
	GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Red, (("count: ") + std::to_string(count)).c_str());
	if(count>=5)
	{
		IsHidden = true;
		GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Green, "hidden");
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
