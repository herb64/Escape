// Copyright Herbert Mehlhose


#include "OpenDoor.h"

// since 4.16, includes are needed to make autocompletion in VS work. API docs
// show, which includes are needed at the end of each sections.
// e.g. http://api.unrealengine.com/INT/API/Runtime/Engine/Engine/UWorld/

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SphereComponent.h"
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

	name = GetOwner()->GetName();
	firstPlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (firstPlayerPawn) {
		UE_LOG(LogTemp, Warning, TEXT("OpenDoor Component: Door = %s, First Player Pawn = %s"),
			*name,
			*firstPlayerPawn->GetName()
		);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NULL PTR for FirstPlayerPawn"), *name);
	}

	if (pressurePlate == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Door %s: missing Pressure Plate Trigger Volume"), *name);
	}

	/* E X P E R I M E N T S 
	 * Just some testing for curiosity here: originally wanted to set the constraints
	 * for physics (lock rotation) here to fix the drifting pawn. But it looks like
	 * this cannot be set, although the Mode for example can be set...
	 * TODO: investigate further at a later point, for now set in editor...
	 */

	// 1. Log a list of all components found for the default pawn actor
	TSet<UActorComponent*> compset;
	compset = firstPlayerPawn->GetComponents();
	for (auto &comp : compset) {
		if (comp) {
			UE_LOG(LogTemp, Warning, TEXT("Component: %s"), *comp->GetName());
		}
	}

	// 2. Get collision component for default pawn, which is a Sphere Component
	USphereComponent* scomp = firstPlayerPawn->FindComponentByClass<USphereComponent>();
	if (scomp) {
		bool bPhysics = scomp->IsSimulatingPhysics(NAME_None);
		// strange: can set constraint mode, or "simulate physics" - but not rotation constraints for axis
		//scomp->SetConstraintMode(EDOFMode::XYPlane);
		UE_LOG(LogTemp, Warning, TEXT("Found Sphere Component: %s, Simulating Physics = %s"), *scomp->GetName(), (bPhysics ? TEXT("True") : TEXT("False")));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NULL PTR for Sphere Component"));
	}

	// 3. Play with Physics Constraint stuff... ccomp not found at all...
	UPhysicsConstraintComponent* ccomp = firstPlayerPawn->FindComponentByClass<UPhysicsConstraintComponent>();
	if (ccomp) {
		FConstraintInstance cinst = ccomp->ConstraintInstance;
		float avgMass = cinst.AverageMass;
		UE_LOG(LogTemp, Warning, TEXT("Mass value: %s"), avgMass);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NULL PTR for Constraint Component"));
	}
}

// Open the door by broadcasting request to Blueprint
void UOpenDoor::OpenDoor()
{
	// Old code for immediate rotation (parameter order: Pitch, Yaw, Roll)
	//GetOwner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OpenRequest.Broadcast();
	bIsOpen = true;
}

// Close the door by broadcasting request to Blueprint
void UOpenDoor::CloseDoor()
{
	CloseRequest.Broadcast();
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
				mass += comp->GetMass();
			}
		}
	}
	
	return mass;
}

