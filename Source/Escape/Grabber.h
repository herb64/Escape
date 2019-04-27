// Copyright Herbert Mehlhose

#pragma once

#include "CoreMinimal.h"

// IWYU
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

// This one always LAST include
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupInputComponent();

	void FindPhysicsHandleComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	APlayerController* playerCtrl;
	/*FVector previousPosition;
	FRotator previousRotation;*/

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Reach"))
	float fReach = 150.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Debugline"))
	bool bDrawDebugLine = true;

	const FHitResult GetFirstPhysicsBodyInReach();
};
