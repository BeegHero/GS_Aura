// Copyright BH


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag) 
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const float NewValue) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = NewValue;
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);

	for (auto& Pair: GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
		.AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			this->BroadcastAttributeInfo(Pair.Key, Data.NewValue);
		});
	}
	
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda([this](int32 Points)
	{
		AttributePointsChangedDelegate.Broadcast(Points);
	});
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 Points)
	{
		SpellPointsChangedDelegate.Broadcast(Points);
	});
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (auto& Pair: GetAuraAS()->TagsToAttributes)
	{
		const float CurrentValue = Pair.Value().GetNumericValue(GetAuraAS());
		BroadcastAttributeInfo(Pair.Key, CurrentValue);
	}
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}
