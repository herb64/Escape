// This is our Custom Actor Component, that can be added to an Actor in the scene
// within UE4 Editor. 
// Copyright Herbert Mehlhose


#include "PositionReport2.h"

// since 4.16, this include is needed to make autocompletion in VS work
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReport2::UPositionReport2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport2::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// FString: mutable
	// see: https://wiki.unrealengine.com/index.php?title=Logs,_Printing_Messages_To_Yourself_During_Runtime
	FString name = GetOwner()->GetName();
	FVector pos = GetOwner()->GetTransform().GetLocation();

	UE_LOG(LogTemp, Warning, TEXT("Hello233 from %s at %s"), *name, *pos.ToString());

}


// Called every frame
void UPositionReport2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

