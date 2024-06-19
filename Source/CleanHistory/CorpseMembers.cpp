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
	//BloodParticle = CreateDefaultSubobject<UNiagaraComponent>("BeamParticles");
	//BeamParticles->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ACorpseMembers::BeginPlay()
{
	Super::BeginPlay();
	MemberMesh->GetSocketLocation(SocketName);
	CutZone->AttachToComponent(MemberMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CutZone->SetHiddenInGame(!Debug);

	
	if (!GetParentComponent())
	{
		hasDetached = true;
		CountAsCorpse = true;
		return;
	}
	CutZone->OnComponentBeginOverlap.AddDynamic(this, &ACorpseMembers::OverlapBegin);
	TArray<USceneComponent*> parents;

	GetParentComponent()->GetParentComponents(parents);
	
	if(parents.IsEmpty())
		return;
	if (!Cast<UChildActorComponent>(parents[0]))
	{
		CountAsCorpse = true;
		return;
	}
	UChildActorComponent* parentChildActor = Cast<UChildActorComponent>(parents[0]);
	ACorpseMembers* ParentCorpseMember = Cast<ACorpseMembers>(parentChildActor->GetChildActor());
	ParentSkelethalMesh = ParentCorpseMember->MemberMesh;

	if (ParentSkelethalMesh == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("The parent value is:"));
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
		CountAsCorpse = true;
		MemberMesh->SetWorldLocation(lastPos);
		MemberMesh->SetSimulatePhysics(true);
		for (size_t i = 0; i < ChildsMesh.Num(); i++)
		{
			if (Cast<UChildActorComponent>(ChildsMesh[i]))
			{
				/*ChildsMesh[i]->SetWorldLocation(ChildsMeshLastPos[i]);
				ChildsMesh[i]->SetLeaderPoseComponent(nullptr);
				ChildsMesh[i]->SetSimulatePhysics(true);*/
				//UChildActorComponent* childActorChild = Cast<UChildActorComponent>(child[i]);
				//ACorpseMembers* childcorpsmembers = Cast<ACorpseMembers>(childActorChild->GetChildActor());
				//ChildsMesh.Add(childcorpsmembers->MemberMesh);
				//ChildsMeshLastPos.Add(childcorpsmembers->MemberMesh->GetComponentToWorld().GetLocation());
				//childcorpsmembers->MemberMesh->SetLeaderPoseComponent(MemberMesh);
				//childmesh = Cast<UChildActorComponent>(child[0])
			}
		}
		hasDetached = false;
		if (bloodManager != nullptr)
		{
			myBloodManager = GetWorld()->SpawnActor<ABloodManager>(bloodManager);
			myBloodManager->AttachToComponent(BleedPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		ChildsMeshLastPos.Empty();
		ChildsMesh.Empty();
	}
	if(MustEject)
	{
		//Eject();
		TArray<FName> bones;
		MemberMesh->GetBoneNames(bones);
		MemberMesh->SetWorldLocation(FVector(100, 100, 100));
		//MemberMesh->GetBoneName()
		for (auto Bone : bones)
		{
			MemberMesh->AddImpulse(FVector(0, 0, 1) * 2000, Bone, true);
		}
		MustEject = false;
	}
	if(Burned)
		return;
	//LineTrace
	//bool ishidedX = false;
	//bool ishidedY = false;
	//bool ishidedZ = false;
	//bool ishidedminusX = false;
	//bool ishidedminusY = false;
	//bool ishidedminusZ = false;
	if(!CountAsCorpse)
		return;
	FHitResult Hitx;
	FHitResult Hity;
	FHitResult Hitz;
	FHitResult Hitminusx;
	FHitResult Hitminusy;
	FHitResult Hitminusz;


	FVector TraceStart = FVector(CutZone->GetComponentToWorld().GetLocation().X, CutZone->GetComponentToWorld().GetLocation().Y, CutZone->GetComponentToWorld().GetLocation().Z);
	FVector TraceEndx = TraceStart+FVector(1, 0, 0) * 100.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredComponent(MemberMesh);
	QueryParams.AddIgnoredComponent(CutZone);
	QueryParams.AddIgnoredComponent(ParentSkelethalMesh);
	bool ishidedX = GetWorld()->LineTraceSingleByChannel(Hitx, TraceStart + FVector(1, 0, 0) * 30.f, TraceEndx,TraceChannelProperty, QueryParams);
	bool ishidedY = GetWorld()->LineTraceSingleByChannel(Hity,  TraceStart + FVector(0, 1, 0) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedZ = GetWorld()->LineTraceSingleByChannel(Hitz,  TraceStart + FVector(0, 0, 1) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedminusX = GetWorld()->LineTraceSingleByChannel(Hitminusx, TraceStart + FVector(-1, 0, 0) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedminusY = GetWorld()->LineTraceSingleByChannel(Hitminusy, TraceStart + FVector(0, -1, 0) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);
	bool ishidedminusZ = GetWorld()->LineTraceSingleByChannel(Hitminusz, TraceStart + FVector(0, 0, -1) * 30.f, TraceEndx, TraceChannelProperty, QueryParams);


	if (Debug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(1, 0, 0) * 30.f, Hitx.bBlockingHit ? FColor::Red : FColor::Magenta, false, .1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, 1, 0) * 30.f, Hity.bBlockingHit ? FColor::Green : FColor::Magenta, false, .1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, 0, 1) * 30.f, Hitz.bBlockingHit ? FColor::Blue : FColor::Magenta, false, .1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(-1, 0, 0) * 30.f, Hitminusx.bBlockingHit ? FColor::Red : FColor::Magenta, false, .1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, -1, 0) * 30.f, Hitminusy.bBlockingHit ? FColor::Green : FColor::Magenta, false, .1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + FVector(0, 0, -1) * 30.f, Hitminusz.bBlockingHit ? FColor::Blue : FColor::Magenta, false, .1f, 0, 2.0f);
	}
	int count = ishidedX + ishidedY + ishidedZ + ishidedminusX + ishidedminusY + ishidedminusZ;
	/*if(ishidedX)
		GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Red, (Hitx.GetActor()->GetFName()).ToString());
	GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Red, (("count: ") + std::to_string(count)).c_str());*/
	if(count>=6)
	{
		IsHidden = true;
		//GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Green, "hidden");
	}
	else
	{
		IsHidden = false;
	}
}

void ACorpseMembers::Die()
{
	if (BurnOnDestroySound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(this, BurnOnDestroySound, GetActorLocation());

	Detache();
	//SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	//SetActorTickEnabled(false);
	Burned= true;
	IsHidden=true;
}

void ACorpseMembers::OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UIWeapon::StaticClass()))
	{
		//if(OtherActor->GetVelocity()>)

		MemberLife -= OtherComp->GetComponentVelocity().Length() * 0.20f;
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		if(MemberLife>0)
			return;
		Detache();
		
	}
}

void ACorpseMembers::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACorpseMembers::Eject()
{
	int32 bonesCount = 0;
	TArray<FName> bones;
	MemberMesh->GetBoneNames(bones);
	MemberMesh->SetWorldLocation(FVector(100, 100, 100));
	//MemberMesh->GetBoneName()
	for (auto Bone : bones)
	{
		MemberMesh->AddImpulse(FVector(0, 0, 1) * 2000, Bone, true);
	}
	
}

void ACorpseMembers::Detache()
{
	if(!GetParentComponent())
		return;
	MemberMesh->SetLeaderPoseComponent(nullptr);
	TArray<USceneComponent*> parents;
	TArray<USceneComponent*> child;
	GetParentComponent()->GetParentComponents(parents);

	child = GetParentComponent()->GetAttachChildren();
	if (!child.IsEmpty())
	{

		for (size_t i = 0; i < child.Num(); i++)
		{
			if (Cast<UChildActorComponent>(child[i]))
			{
				UChildActorComponent* childActorChild = Cast<UChildActorComponent>(child[i]);
				ACorpseMembers* childcorpsmembers = Cast<ACorpseMembers>(childActorChild->GetChildActor());
				childcorpsmembers->Detache();
				
				ChildsMesh.Add(childcorpsmembers->MemberMesh);
				ChildsMesh[i]->SetSimulatePhysics(false);
				ChildsMeshLastPos.Add(childcorpsmembers->MemberMesh->GetComponentToWorld().GetLocation());
				//childcorpsmembers->MemberMesh->SetLeaderPoseComponent(MemberMesh);
				//childmesh = Cast<UChildActorComponent>(child[0])
			}
		}

	}
	/*if (!child.IsEmpty())
	{

		for (size_t i = 0; i < child.Num(); i++)
		{
			if (Cast<UChildActorComponent>(child[i]))
			{
				UChildActorComponent* childActorChild = Cast<UChildActorComponent>(child[i]);
				ACorpseMembers* childcorpsmembers = Cast<ACorpseMembers>(childActorChild->GetChildActor());
				childcorpsmembers->MemberMesh->SetLeaderPoseComponent(MemberMesh);
				//childmesh = Cast<UChildActorComponent>(child[0])
			}
		}

	}*/
	lastPos = MemberMesh->GetComponentToWorld().GetLocation();
	if (parents.IsEmpty())
		return;
	if (!Cast<UChildActorComponent>(parents[0]))
		return;
	UChildActorComponent* test = Cast<UChildActorComponent>(parents[0]);
	//test->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetParentComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MemberMesh->SetSimulatePhysics(false);
	hasDetached = true;
	UGameplayStatics::PlaySoundAtLocation(this, DismenberSound, GetActorLocation());


	CutZone->OnComponentBeginOverlap.RemoveDynamic(this, &ACorpseMembers::OverlapBegin);
	if (GetParentComponent()->GetChildComponent(0))
	{

	}
}

void ACorpseMembers::DetacheAll()
{
	MemberMesh->SetLeaderPoseComponent(nullptr);


	TArray<USceneComponent*> parents;
	TArray<USceneComponent*> child;
	GetParentComponent()->GetParentComponents(parents);

	child = GetParentComponent()->GetAttachChildren();

	if (!child.IsEmpty())
	{

		for (size_t i = 0; i < child.Num(); i++)
		{
			if (Cast<UChildActorComponent>(child[i]))
			{
				UChildActorComponent* childActorChild = Cast<UChildActorComponent>(child[i]);
				ACorpseMembers* childcorpsmembers = Cast<ACorpseMembers>(childActorChild->GetChildActor());
				childcorpsmembers->MemberMesh->SetLeaderPoseComponent(MemberMesh);
				childcorpsmembers->Detache();
				//childcorpsmembers->MemberMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				//childcorpsmembers->MemberMesh->SetSimulatePhysics(false);
				//childcorpsmembers->hasDetached = true;

				//childmesh = Cast<UChildActorComponent>(child[0])
			}
		}

	}

	if (parents.IsEmpty())
		return;
	if (!Cast<UChildActorComponent>(parents[0]))
		return;

	lastPos = MemberMesh->GetComponentToWorld().GetLocation();
	MemberMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MemberMesh->SetSimulatePhysics(false);
	hasDetached = true;
	UGameplayStatics::PlaySoundAtLocation(this, DismenberSound, GetActorLocation());


	CutZone->OnComponentBeginOverlap.RemoveDynamic(this, &ACorpseMembers::OverlapBegin);
	if (GetParentComponent()->GetChildComponent(0))
	{

	}
}

