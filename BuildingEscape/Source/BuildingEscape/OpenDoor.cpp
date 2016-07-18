// Copyrigth Shaquille

#include "BuildingEscape.h"
#include "OpenDoor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT(" %s Missing pressure plate"), *GetOwner()->GetName())//This gets the name of the object
																						   //thats missing the component
	}

}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


	//Poll the Trigger Volume
    if(GetTotalMassOfActorOnPlate() > TriggerMass)
    {
		OnOpen.Broadcast();
        
    }
	else{

		OnClose.Broadcast();
    }
}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
    float TotalMass = 0.f;
    
    //Find all the overlapping actor
    TArray<AActor*> OverlappingActor;
	if (!PressurePlate) { return TotalMass; }
    PressurePlate->GetOverlappingActors(OUT OverlappingActor);
    
    
    //Iterate through them assing their masses
    
    for (const auto& Actor : OverlappingActor)
    {
       TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
    }
    
    return TotalMass;
}





