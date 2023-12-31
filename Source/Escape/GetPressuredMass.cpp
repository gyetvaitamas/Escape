#pragma once

#include "GetPressuredMass.h"
// Fill out your copyright notice in the Description page of Project Settings.

#define OUT

// Sets default values for this component's properties
UGetPressuredMass::UGetPressuredMass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGetPressuredMass::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate component!"), *GetOwner()->GetName());
	}

}

float UGetPressuredMass::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	if (!PressurePlate)
	{
		return TotalMass;
	}

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;

}


// Called every frame
void UGetPressuredMass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FText MassAsText = FText::AsNumber(GetTotalMassOfActorsOnPlate());

	FText ConcatenatedText = FText::Format(FText::FromString("{0}{1}"), MassAsText, FText::FromString(" KG"));

	// Change mass indicator every tick
	GetOwner()->FindComponentByClass<UTextRenderComponent>()->SetText(ConcatenatedText);

}
