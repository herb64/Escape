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

	// Note: it looks like if setting values for editable items here, they are used
	// in editor as default, but editor changes are not reflected when playing.
	// ?? https://forums.unrealengine.com/development-discussion/c-gameplay-programming/122958-uproperty-editanywhere-variable-when-changed-in-the-editor-dont-reflect-in-the-game
	// Removing the UPROPERTY statements, recompile, then readd UPROPERTY statements again
	// and everything works fine!
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Reach"))
	float fReach = 200;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Draw Debug Line"))
	bool bDrawDebugLine = true;

	void Grab();
	void Release();
	const FHitResult GetFirstPhysicsBodyInReach();
	const FTwoVectors getLineTracePoints();
};
