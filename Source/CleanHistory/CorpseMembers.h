// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CorpseMembers.generated.h"
class USkeletalMesh;
UCLASS()
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
	TObjectPtr< USkeletalMesh> ParentSkelethalMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> MemberMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> CutZone;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> BleedPoint;
};
