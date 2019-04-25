// Copyright Herbert Mehlhose

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// include trigger volume (need this because of IWYU)
#include "Engine/TriggerVolume.h"

// this must be last include always
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
// new private stuff for triggerplate and door handling
private:

	/// VisibleAnywhere - not editable
	/// UPROPERTY(VisibleAnywhere)
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate;

	// make a member variable used by open/close instead of doing a query each time
	AActor* owner;
	FString name;
	bool bIsOpen;

	// Time interval, after which the door gets closed after leaving the trigger volume
	UPROPERTY(EditAnywhere, meta=(DisplayName="Close Delay"))
	float fCloseDelay = 1.0f;
	
	float fLastOpenTime;

	// This is the actor of the player - this is the pawn, casted as
	// actor. Allowed, because a pawn IS AN ACTOR
	AActor* firstPlayerPawn;
};
