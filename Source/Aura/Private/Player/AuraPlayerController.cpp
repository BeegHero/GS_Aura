// Copyright BH


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Get Subsystem for EnhancedInput
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	//Cursor Behaviour
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//Not Sure, setting InputMode behaviour for Cursor?
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//Get the movement represented in 2d vector
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	//Get Rotation, even though we haven't added any rotation yet
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//Get the pawn, add the movement
	if( APawn* ControlledPawn = GetPawn<APawn>())
	{
		//Y represents the W/S keys, X represents the A/D keys
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from Cursor, multiple scenarios:
	 * A. LastActor and ThisActor is Null
	 *	- Do Nothing
	 * B. LastActor is Null. ThisActor is valid.
	 *  - Hovering over ThisActor first time, thus highlight actor.
	 * C. LastActor is Valid. ThisActor is null.
	 *  - UnHighlight LastActor
	 * D. Both Actors are Valid, but LastActor != ThisActor
	 *  - UnHighlight LastActor, Highlight ThisActor
	 * E. Both Actors are Valid, but LastActor != ThisActor
	 *  - Hovering over the same object, do Nothing
	 */

	if (LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			//Case B
			ThisActor->HighlightActor();
		}
		else
		{
			//Case A, both are null, do nothing
		}
	}
	else
	{
		if(ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHighlightActor();
		}
		else
		{
			if(LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//Case E
				//Do Nothing
			}
		}
	}
}
