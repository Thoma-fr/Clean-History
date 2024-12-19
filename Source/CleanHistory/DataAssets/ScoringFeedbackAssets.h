#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ScoringFeedbackAssets.generated.h"

UCLASS()
class CLEANHISTORY_API UScoringFeedbackAssets : public UDataAsset
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int, FString> feedBackFromTreshhold;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int, UNiagaraSystem*> particuleFromTreshhold;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float specialFeedBackDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ScoreSpecialFeedbackWidgetClass;
};

