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

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
    
}
//Looks for attach physics handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    
    if(PhysicsHandle)
    {
        //Physics handle is find
    }
    else{
        UE_LOG(LogTemp, Error, TEXT(" %s Missing physics handle component"), *GetOwner()->GetName())//This gets the name of the object
        //thats missing the physcis handle
    }

}

//Looks for Input component
void UGrabber::SetupInputComponent(){
    
 
    Input = GetOwner()->FindComponentByClass<UInputComponent>();
    
    if(Input)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"));
        ///Bind the input axis
        Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else{
        UE_LOG(LogTemp, Error, TEXT(" %s Missing input component"), *GetOwner()->GetName())//This gets the name of the object
        //thats missing the component
    }
    

}


void UGrabber::Grab(){
    
    UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
    
    //LINE TRACE and see if we reach any actor why physics body collision channel set
    auto HitResut = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResut.GetComponent();
    auto ActorHit = HitResut.GetActor();
    
    //if we hit something then attach a physics handle
    if(ActorHit)
    {
        //Attach physics handle
        PhysicsHandle->GrabComponent(
                                 ComponentToGrab,
                                 NAME_None,
                                 ComponentToGrab->GetOwner()->GetActorLocation(),
                                 true//Allow rotation
                                 );
    }
    
}

void UGrabber::Release(){
   
    UE_LOG(LogTemp, Warning, TEXT("Grab Release"));
    PhysicsHandle->ReleaseComponent();//ReleaseComponent
    
  
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

    
    ////if the physics handle attached
    if(PhysicsHandle->GrabbedComponent)
    {
        // move the object thet we're holding
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){
    
    //Get player view point this tick(Where they are and looking)
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
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
    
    return Hit;
    
}

