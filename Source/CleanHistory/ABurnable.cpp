#include "ABurnable.h"

void AABurnable::ApplyDamageOverTime(uint32 DamagePerSecond)
{
    if (Health > 0)
    {
        Damage = DamagePerSecond;

        GetWorldTimerManager().SetTimer(DamageTimer, this, &AABurnable::DealDamagePerSecond, 1.0f, true, 1.0f);
    }
}

void AABurnable::DealDamagePerSecond()
{

    Health -= Damage;

    if (Health <= 0)
    {
        SetActorEnableCollision(false);
        SetActorHiddenInGame(true);
        SetActorTickEnabled(false);

        GetWorldTimerManager().ClearTimer(DamageTimer);
    }
}