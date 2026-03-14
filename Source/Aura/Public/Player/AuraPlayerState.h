// Copyright BH

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChange, int32 /* Stat Value */);

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const;

	FORCEINLINE int32 GetPlayerLevel() const { return Level;}
	
	FORCEINLINE int32 GetXP() const { return XP;}
	
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints;}
	
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints;}
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStatChange OnXPChangedDelegate;
	FOnPlayerStatChange OnLevelChangedDelegate;
	FOnPlayerStatChange OnAttributePointsChangedDelegate;
	FOnPlayerStatChange OnSpellPointsChangedDelegate;
	
	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	
	void AddXP(int32 NewXP);
	void AddLevel(int32 NewLevel);
	
	void SetAttributePoints(int32 NewAttributePoints);
	void AddAttributePoints(int32 InAttributePoints);
	
	void SetSpellPoints(int32 NewSpellPoints);
	void AddSpellPoints(int32 InSpellPoints);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 1;
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 1;
	
};
