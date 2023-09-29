#pragma once

#include "CoreMinimal.h"
#include "OpenDoor.h"
#include "Components/ActorComponent.h"
#include <Engine/TriggerVolume.h>
#include <Engine/TextRenderActor.h>
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 70.0f;

	// The owning door
	AActor* Owner = nullptr;

	// Returns total mass of kg
	float GetTotalMassOfActorsOnPlate();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
