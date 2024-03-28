// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CorpseMembers.generated.h"

class ABloodManager;
class USkeletalMesh;
UCLASS(Blueprintable)
class CLEANHISTORY_API ACorpseMembers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACorpseMembers();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> ParentSkelethalMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MemberMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> CutZone;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USceneComponent> BleedPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABloodManager> bloodManager;

	UPROPERTY()
	TObjectPtr<ABloodManager> myBloodManager;
	//UPROPERTY()
	UPROPERTY(EditAnywhere)
	FName SocketName;

	/*UFUNCTION()
		void ChangeParent(USkeletalMesh* parent);*/
	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
