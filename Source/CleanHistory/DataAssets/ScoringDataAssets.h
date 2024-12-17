#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ScoringDataAssets.generated.h"

UENUM(BlueprintType)
enum class EScoringTypeEnum : uint8
{
	DESTRUCTION,
	CUT,
	BLOOD,
	BURN,
	DEFAULT
};

UCLASS()
class CLEANHISTORY_API UScoringDataAssets : public UDataAsset
{
	GENERATED_BODY()

public :

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EScoringTypeEnum, float> scoreTypeValues;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EScoringTypeEnum, float> scoreTypeMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ScoreFeedbackWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float comboDelay;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ScoreFeedbackTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UNiagaraSystem* BonusparticuleFeedback;
};

