// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CleanHistory/Interfaces/IBurnable.h"
#include "GameFramework/Actor.h"
#include "Brasero.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyItemBurn, FName, ItemName);
UCLASS()
class CLEANHISTORY_API ABrasero : public AActor
{

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrasero();

protected:
	// Called when the game starts or when spawned 
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Burn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* BurnActorSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FireExstinctSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* EjectSound;
	UPROPERTY(EditAnywhere)
	uint32 DamagePerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NbBurnbaleObject = 5;
	UPROPERTY(BlueprintAssignable)
	FOnKeyItemBurn OnKeyItemBurn;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	bool CanBurn =  true;

	UPROPERTY(EditAnywhere)
	bool EjectAtEnd = false;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<class UBoxComponent> CollisionBoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	AActor* ActorInZone = nullptr;
};
