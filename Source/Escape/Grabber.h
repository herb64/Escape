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

	void FindFirstPlayerController();

	void SetupInputComponent();

	void FindPhysicsHandleComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	APlayerController* playerCtrl = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Reach"))
	float fReach = 150.0f;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Debugline"))
	bool bDrawDebugLine = true;

	void Grab();
	void Release();
	const FHitResult GetFirstPhysicsBodyInReach();
	const FTwoVectors getLineTracePoints();
};
