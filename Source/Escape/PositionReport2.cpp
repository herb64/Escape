// Custom C++ Actor Component to report current Position in log on BeginPlay
// Copyright Herbert Mehlhose


#include "PositionReport2.h"

// since 4.16, this include is needed to make autocompletion in VS work
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReport2::UPositionReport2()
{
	// Setting tick to false here for performance reasons. We only report once
	// on BeginPlay, no tick needed.
	// It's not that simple, log still written.
	// https://answers.unrealengine.com/questions/601078/cannot-disable-actorcomponent-ticking.html
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPositionReport2::BeginPlay()
{
	Super::BeginPlay();

	FString name = GetOwner()->GetName();
	FVector pos = GetOwner()->GetTransform().GetLocation();
	UE_LOG(LogTemp, Warning, TEXT("Hello from %s at %s"), *name, *pos.ToString());
}


// Called every frame
void UPositionReport2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("TICK PositionReport2"));
}

