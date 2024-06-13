// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodShaderManager.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
ABloodShaderManager::ABloodShaderManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UTextureRenderTarget2D* ABloodShaderManager::CreateRenderTargetOpti(UWorld* World, int32 Width, int32 Height, 
    EPixelFormat Format, bool bClearColor, bool bAutoGenerateMipMaps, bool bSupportUAVs)
{
    // Créer une nouvelle instance de UTextureRenderTarget2D
    UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();

    // Configurer la largeur et la hauteur
    RenderTarget->InitCustomFormat(Width, Height, Format, bClearColor);

    // Configurer les options supplémentaires
    RenderTarget->bAutoGenerateMips = bAutoGenerateMipMaps;
    RenderTarget->bGPUSharedFlag = bSupportUAVs;

    RenderTarget->bGPUSharedFlag = true;
    RenderTarget->bHDR_DEPRECATED = false;

    // Enregistrer l'objet pour qu'il ne soit pas nettoyé par le garbage collector
    RenderTarget->AddToRoot();

    return RenderTarget;
}

// Called when the game starts or when spawned
void ABloodShaderManager::BeginPlay()
{
	Super::BeginPlay();
	
}