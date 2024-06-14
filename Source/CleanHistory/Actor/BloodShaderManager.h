// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BloodShaderManager.generated.h"

enum ETextureRenderTargetFormat : int;

UCLASS()
class CLEANHISTORY_API ABloodShaderManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodShaderManager();

	UFUNCTION(BlueprintCallable, Category = "BloodHandleFunctions")
	UTextureRenderTarget2D* CreateRenderTargetOpti(UWorld* World, int32 Width, int32 Height,
		EPixelFormat Format, bool bClearColor, bool bAutoGenerateMipMaps, bool bSupportUAVs);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
