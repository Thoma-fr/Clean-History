#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IBurnable.h"
#include "ABurnable.generated.h"

UCLASS(Abstract)
class CLEANHISTORY_API AABurnable : public AActor, public IIBurnable
{
    GENERATED_BODY()

public:
    virtual void ApplyDamageOverTime(uint32 DamagePerSecond);

    virtual void DealDamagePerSecond();

private:
    UPROPERTY(EditAnywhere)
    uint32 Health;
    
    UPROPERTY(EditAnywhere)
    uint32 Damage;

    UPROPERTY()
    FTimerHandle DamageTimer;
};
