// Copyright BH


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const float NewValue) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = NewValue;
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair: AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
		.AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			this->BroadcastAttributeInfo(Pair.Key, Data.NewValue);
		});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair: AS->TagsToAttributes)
	{
		const float CurrentValue = Pair.Value().GetNumericValue(AS);
		BroadcastAttributeInfo(Pair.Key, CurrentValue);
	}
}
