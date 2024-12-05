#pragma once

#include "Subsystems/Subsystem.h"
#include "Blueprint/UserWidget.h" 
#include "../DataAssets/ScoringDataAssets.h"
#include "../DataAssets/ScoringFeedbackAssets.h"
#include "../Actor/Prop.h"
#include "ScoringSubSystem.generated.h"

UCLASS()
class CLEANHISTORY_API UScoringSubSystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void Tick(float DeltaSeconds) override;

	void DisplayScoreFeedback(float Score, FVector WorldLocation);

	void DisplaySpecialFeedback(FString specialText, FVector WorldLocation);

	UFUNCTION(BlueprintCallable)
	void Score(EScoringTypeEnum scoreType, FVector WorldLocation);

	UFUNCTION(BlueprintCallable)
	void ScoreFromProp(EScoringTypeEnum scoreType, FVector WorldLocation, AProp* prop);

	UFUNCTION(BlueprintCallable)
	void ScoreWithValue(EScoringTypeEnum scoreType, FVector WorldLocation, float ScoreValue, float ScoreMultiplier, bool isEvent);

	UFUNCTION(BlueprintCallable)
	void ScoreWithValueAndText(EScoringTypeEnum scoreType, FVector WorldLocation, float ScoreValue, float ScoreMultiplier, bool isEvent, FString specialText);

	UFUNCTION(BlueprintCallable)
	int GetFinalScore();

	UFUNCTION(BlueprintCallable)
	int GetScore(EScoringTypeEnum scoreType);

private:
	
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UScoringDataAssets* scoringDataAsset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UScoringFeedbackAssets* scoringFeedbackAsset;

private: 
	UPROPERTY()
	int scoreDestruction = 0;

	UPROPERTY()
	int scoreCut = 0;

	UPROPERTY()
	int scoreBlood = 0;

	UPROPERTY()
	int scoreBurn = 0;

	UPROPERTY()
	float multiplier = 1;

	UPROPERTY()
	EScoringTypeEnum lastType;
	
	UPROPERTY()
	float comboTime = 0;

	UPROPERTY()
	float specialFeedbackTime = 0;

	// Hérité via FTickableGameObject
	TStatId GetStatId() const override;
};