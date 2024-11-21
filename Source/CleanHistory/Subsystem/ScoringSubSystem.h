#pragma once

#include "Subsystems/Subsystem.h"
#include "Blueprint/UserWidget.h" 
#include "ScoringSubSystem.generated.h"

UENUM(BlueprintType)
enum class EScoringTypeEnum : uint8
{
	DESTRUCTION,
	CUT,
	BLOOD,
	BURN,
	DEFAULT
};

UCLASS(Blueprintable)
class UScoringSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void Tick(float DeltaSeconds);

	void DisplayScoreFeedback(float Score, FVector WorldLocation);

	UFUNCTION(BlueprintCallable)
	void Score(EScoringTypeEnum scoreType, FVector WorldLocation);

	UFUNCTION(BlueprintCallable)
	int GetFinalScore();

	UFUNCTION(BlueprintCallable)
	int GetScore(EScoringTypeEnum scoreType);

	UFUNCTION(BlueprintCallable)
	void SetComboDelay(float delay);

private:
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EScoringTypeEnum, float> scoreTypeValues;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EScoringTypeEnum, float> scoreTypeMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float comboDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ScoreFeedbackWidgetClass;

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
};