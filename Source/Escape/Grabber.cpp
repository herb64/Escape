// Copyright Herbert Mehlhose


#include "Grabber.h"

// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"		// e.g. DrawDebugLine
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"


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

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// New code will just check for an attached physics handle and move the object we carry. 
	if (PhysicsHandle->GrabbedComponent) {
		//PhysicsHandle->SetTargetLocation(getLineTraceEndPoint());
		PhysicsHandle->SetTargetLocation(getLineTracePoints().v2);
	}
}

// Grab function to pick an object
void UGrabber::Grab()
{
	/// get any actors with physics body collision channel set
	/// if found: attach a physics handle
	/// TIP: using auto to determine type
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	// TODO: fixed rotator for now - make rotation to face ourselves always.
	if (ActorHit) {
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator(0.0f, 0.0f, 0.0f)
		);
		UE_LOG(LogTemp, Warning, TEXT("Attaching Physics Handle for hit actor: %s"), *ActorHit->GetName());
	}
}

/// Release a picked object
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}



/// Prepare the input component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		//FInputActionBinding tmpBinding;

		InputComponent->BindAction(
			"Grab",
			IE_Pressed,
			this,
			&UGrabber::Grab
		);

		InputComponent->BindAction(
			"Release",
			IE_Pressed,
			this,
			&UGrabber::Release
		);

		//UE_LOG(LogTemp, Warning, TEXT("Bound Action: %s"), *tmpBinding.GetActionName().ToString());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No input component attached to default pawn!"));
	}
}

/// Get Physics Handle Component
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No physics handle component on default pawn!"));
	}
}

/// Get the first Physics Body object in our reach
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/*FVector position;
	FVector endpoint;
	getLineTracePoints(
		OUT position, 
		OUT endpoint
	);*/

	FTwoVectors traceLinePoints = getLineTracePoints();

	// see also https://wiki.unrealengine.com/Draw_3D_Debug_Points,_Lines,_and_Spheres:_Visualize_Your_Algorithm_in_Action
	// Now conditional config in BP and using lifetime now to keep it visible
	if (bDrawDebugLine) {
		DrawDebugLine(
			GetWorld(),
			traceLinePoints.v1,
			traceLinePoints.v2,
			FColor::Yellow,
			false,
			4.0f,
			0,
			3.0f);
	}

	/// prepare a collision check for object type "PhysicsBody", which is set when
	/// enabling Physics.
	FCollisionQueryParams QueryParms(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		traceLinePoints.v1,
		traceLinePoints.v2,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParms
	);

	return Hit;
}


/// LINE TRACING STUFF - better might be using FTwoVectors as return, avoiding passing references and changing them??
/// https://api.unrealengine.com/INT/API/Runtime/Core/Math/FTwoVectors/index.html
const FTwoVectors UGrabber::getLineTracePoints()
{
	FVector position;
	FRotator rotation;
	playerCtrl->GetPlayerViewPoint(
		OUT position,
		OUT rotation
	);
	return FTwoVectors(position, position + rotation.Vector() * fReach);
}

///// OLD CODE - replace by singe code using FTwoVectors
// *Just getting the endpoint of a trace line. Variable fReach is used to determine length of vector!
const FVector UGrabber::getLineTraceEndPoint() {
	FVector position;
	FRotator rotation;
	playerCtrl->GetPlayerViewPoint(
		OUT position,
		OUT rotation
	);
	return position + rotation.Vector() * fReach;
}

// *Getting both, starting and end point of line trace. Variable fReach is used to determine length of vector!
void UGrabber::getLineTracePoints(FVector &pos, FVector &end)
{
	FVector position;
	FRotator rotation;
	playerCtrl->GetPlayerViewPoint(
		OUT position,
		OUT rotation
	);
	pos = position;
	end = position + rotation.Vector() * fReach;
}
