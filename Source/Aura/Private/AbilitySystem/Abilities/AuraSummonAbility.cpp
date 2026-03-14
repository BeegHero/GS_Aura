// Copyright BH


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	TArray<FVector> SpawnLocations;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	const float DeltaSpread = SpawnSpread / (NumMinions - 1);
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread/2.f, FVector::UpVector);
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);
		FVector ChosenLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, ChosenLocation + FVector(0.f, 0.f, 500.f), 
			ChosenLocation - FVector(0.f, 0.f, 500.f), ECollisionChannel::ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			ChosenLocation = HitResult.ImpactPoint;
		}
		SpawnLocations.Add(ChosenLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
