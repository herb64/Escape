// Copyright Herbert Mehlhose


#include "Grabber.h"

// Since 4.16, includes are needed to make autocompletion in VS work. 
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


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
	if (!position.Equals(previousPosition) || !(rotation.Equals(previousRotation)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber POS: %s, ROT: %s"), *position.ToString(), *rotation.ToString());
		previousPosition = position;
		previousRotation = rotation;
	}
	
}

