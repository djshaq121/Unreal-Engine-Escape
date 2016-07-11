// Copyrigth Shaquille

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
    
    void OpenDoor();
    void CloseDoor();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
    //This keeps the variable private but visiable in the unreal engine
    UPROPERTY(EditAnywhere)
    float OpenAngle = -90.0f;
    
    //A trigger volume, is a invisvible volume in the game world that can be used to tell code to do something 
    UPROPERTY(EditAnywhere)
    ATriggerVolume  *PressurePlate;
    
    //This is a time delay.
    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 1.0f;
    
    float LastDoorOpenTime;
   
   
    AActor* Owner;
    
    //Returns total mass in kg
    float GetTotalMassOfActorOnPlate();
};
