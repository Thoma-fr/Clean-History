// Fill out your copyright notice in the Description page of Project Settings.

#include "Brasero.h"

#include "Kismet/GameplayStatics.h"
#include "CleanHistory/CorpseMembers.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABrasero::ABrasero()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	StaticMeshComponent->Mobility = EComponentMobility::Static;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->bUseDefaultCollision = true;
	RootComponent = StaticMeshComponent;

	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBoxComponent->SetupAttachment(RootComponent);
	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABrasero::OnBeginOverlap);
	CollisionBoxComponent->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ABrasero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrasero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrasero::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(!CanBurn && EjectAtEnd)
	{
		Cast<ACorpseMembers>(OtherActor)->Eject();
	}
	if (OtherActor->GetClass()->ImplementsInterface(UIBurnable::StaticClass()))
	{
		ActorInZone = OtherActor;
		ABrasero::Burn();
	}
}

void ABrasero::Burn() 
{
	if (!CanBurn)
		return;

	IIBurnable* BurnableActor = Cast<IIBurnable>(ActorInZone);
	if (BurnableActor)
	{
		if (BurnActorSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(this, BurnActorSound, GetActorLocation());

		BurnableActor->ApplyDamageOverTime(DamagePerSecond);
	}

	if (--NbBurnbaleObject <= 0)
	{
		CanBurn = false;
		if(!EjectAtEnd)
			CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}