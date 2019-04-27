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
	/*previousPosition = FVector(0.f, 0.f, 0.f);
	previousRotation = FRotator(0.f, 0.f, 0.f);*/

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// New code will just check for an attached physics handle and move
	/// the object we carry. 

	/// REST of code now in GetFirstPhysicsBodyInReach

	

}

/// Grab function to pick an object
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grab action triggered...."));
	/// get any actors with physics body collision channel set
	/// if found: attach a physics handle
	GetFirstPhysicsBodyInReach();
}

/// Grab function to pick an object
void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Release action triggered...."));
	/// If physics handle attached: detach
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
	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Physics handle of default pawn: %s"), *PhysicsHandle->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No physics handle component on default pawn!"));
	}
}

/// Get the first Physics Body object in our reach
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector position;
	FRotator rotation;
	playerCtrl->GetPlayerViewPoint(
		OUT position,
		OUT rotation
	);

	FVector endpoint = position + rotation.Vector() * fReach;

	// see also https://wiki.unrealengine.com/Draw_3D_Debug_Points,_Lines,_and_Spheres:_Visualize_Your_Algorithm_in_Action
	// Now conditional config in BP and using lifetime now to keep it visible
	if (bDrawDebugLine) {
		DrawDebugLine(
			GetWorld(),
			position,
			endpoint,
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
	return FHitResult();
}

