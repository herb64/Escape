// Copyright Herbert Mehlhose


#include "OpenDoor.h"

// since 4.16, includes are needed to make autocompletion in VS work. API docs
// show, which includes are needed at the end of each sections.
// e.g. http://api.unrealengine.com/INT/API/Runtime/Engine/Engine/UWorld/

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// For performance reason: set false if tick is not needed!
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Get the owner actor and name into private variables
	owner = GetOwner();
	name = owner->GetName();
	firstPlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("OpenDoor Component Constructor\nDoor: %s\nFirst Player Pawn: %s"), 
		*name, 
		*firstPlayerPawn->GetName()
	);

	if (pressurePlate == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No pressure plate for door %s"), *name);
	}
}

// TIP:
// This function has been created by selecting code lines, right click and doing a refactor
// into a separate function.
void UOpenDoor::OpenDoor()
{
	// Just some test code
	FRotator rotator = owner->GetActorRotation();
	float fRotationZ = rotator.GetComponentForAxis(EAxis::Z);
	rotator.SetComponentForAxis(EAxis::Z, -50.0);

	// NOTE: parameter order: Pitch, Yaw, Roll
	// Actor StaticMeshComponent needs to be "Movable" in order to be rotated!
	owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	bIsOpen = true;
}

// Close the door
void UOpenDoor::CloseDoor()
{
	owner->SetActorRotation(FRotator(0.f, 180.f, 0.f));
	bIsOpen = false;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressurePlate == nullptr) { return; }

	if (GetTotalMassOnPressurePlate() >= 40.0)
	{
		// We are overlapping: if door is closed, just open it
		if (!bIsOpen)
		{
			OpenDoor();
		}
		// In any case, set our time
		fLastOpenTime = GetWorld()->GetTimeSeconds();
	}
	else {
		// Not overlapping: if door is open, we have left the volume
		if (bIsOpen) {
			// If delay exceeded: close the door
			if (GetWorld()->GetTimeSeconds() - fLastOpenTime > fCloseDelay) 
			{
				CloseDoor();
			}
		}
	}

}

/// Add up mass for all Actors overlapping with the TriggerVolume
/// https://docs.unrealengine.com/en-US/Programming/UnrealArchitecture/TArrays
/// Make sure, all Actors have "Generate Overlap Events" checked in Collision settings!
/// TODO: sometimes defaultpawn gets on moving slowly towards door wall... enlessly			PROBLEM - SAME DRIFTING IN COURSE AS WELL
const float UOpenDoor::GetTotalMassOnPressurePlate()
{
	float mass = 0.0f;
	TArray<AActor*> triggerArray;
	if (pressurePlate) {
		pressurePlate->GetOverlappingActors(triggerArray);
		for (auto &actor : triggerArray)
		{
			UPrimitiveComponent* comp = actor->FindComponentByClass<UPrimitiveComponent>();
			if (comp) {
				//UE_LOG(LogTemp, Error, TEXT("Array Element: %s"), *Iterator->GetName());
				mass += comp->GetMass();
			}
		}
	}
	
	return mass;
}

