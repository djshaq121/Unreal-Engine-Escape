// Copyrigth Shaquille

#include "BuildingEscape.h"
#include "Grabber.h"
#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
    
    //Looks for attach physics handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    
    if(PhysicsHandle)
    {
        //Physics handle is find
    }
    else{
        UE_LOG(LogTemp, Error, TEXT(" %s Missing physics handle component"), *GetOwner()->GetName())//This gets the name of the object
        //thats missing the physcis handle
    }
    
    
    //Looks for Input component
    Input = GetOwner()->FindComponentByClass<UInputComponent>();
    
    if(Input)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"));
        ///Bind the input axis
        Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    }
    else{
        UE_LOG(LogTemp, Error, TEXT(" %s Missing input component"), *GetOwner()->GetName())//This gets the name of the object
        //thats missing the component
    }

    
}

void UGrabber::Grab(){
     UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
    
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Get player view point this tick(Where they are and looking)
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
     );
    
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    //Draw a red line in the world to visualise
    DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255,0,0),false,0.f,0.f,10.f);
  
    
    //Setup query parameters
    //We have false because we dont want visbility collision rather have player collision, and we have getowner to ignore our self so we can see
    //the object in front of us and not our self
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    //We are sorting by object type
    //Line trace (AKA Ray-Cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            PlayerViewPointLocation,
                                            LineTraceEnd,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    //see what we hit
    AActor *ActorHit = Hit.GetActor();
    if(ActorHit)
    {
        //This gets the name of the object we look at if it simulates physics
        UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));

    }
    
}

