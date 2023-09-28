// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/TriggerVolume.h>
#include "GetPressuredMass.generated.h"

class UTextRenderComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGetPressuredMass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGetPressuredMass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	// Returns total mass of kg
	float GetTotalMassOfActorsOnPlate();

	// The owning door
	AActor* Owner = nullptr;

	UTextRenderComponent* MassText;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
