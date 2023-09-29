#pragma once

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}

// Look for attached InputComponent (on run-time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s class is not found!"), *GetOwner()->GetName());
	}

	return;

}

// Look for attached PhysicsHandle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName());
	}

	return;

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName());
		return;
	}

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}

	return;

}

FVector UGrabber::GetLineTraceStart()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;

}

FVector UGrabber::GetLineTraceEnd()
{
	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// Get line end point from the player POV
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	return LineTraceEnd;

}

void UGrabber::Grab()
{
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName());
		return;
	}

	// Line trace and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// If we hit something
	if (ActorHit != nullptr)
	{
		// Attach the physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab, // The mesh to grab
			NAME_None, // No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allow rotation
		);
	}

	return;

}

void UGrabber::Release()
{
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName());
		return;
	}

	PhysicsHandle->ReleaseComponent();

	return;

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Draw a red trace in the world to visualize
	// DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	// Trace line hit result
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult HitResult;

	// When collide
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;

}
