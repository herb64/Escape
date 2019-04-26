// Copyright Herbert Mehlhose


#include "Grabber.h"

// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"		// e.g. DrawDebugLine


// empty OUT macro to annotate passed references as output parameters which
// get modified on return of a called function
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber for %s"), *GetOwner()->GetName());
	playerCtrl = GetWorld()->GetFirstPlayerController();
	previousPosition = FVector(0.f, 0.f, 0.f);
	previousRotation = FRotator(0.f, 0.f, 0.f);
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector position;
	FRotator rotation;
	playerCtrl->GetPlayerViewPoint(
		OUT position,
		OUT rotation
	);

	// Log pos/rot if changed to previous tick
	/*if (!position.Equals(previousPosition) || !(rotation.Equals(previousRotation)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber POS: %s, ROT: %s"), *position.ToString(), *rotation.ToString());
		previousPosition = position;
		previousRotation = rotation;
	}*/
	
	FVector endpoint;
	endpoint = position + rotation.Vector() * fReach;

	// see also https://wiki.unrealengine.com/Draw_3D_Debug_Points,_Lines,_and_Spheres:_Visualize_Your_Algorithm_in_Action
	DrawDebugLine(
		GetWorld(),
		position,
		endpoint,
		FColor::Yellow,
		false,
		-1.0f,
		0,
		3.0f);

	/// prepare a collision check for object type "PhysicsBody", which is set when
	/// enabling Physics.
	FCollisionQueryParams QueryParms(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		position,
		endpoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParms
	);
	
	/// This is still ugly, we might make this nicer to report only changes of hitActor
	AActor* hitActor = Hit.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *hitActor->GetName());
	}

}

