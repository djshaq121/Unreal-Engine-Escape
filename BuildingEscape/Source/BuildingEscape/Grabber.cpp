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
    
    if(PhysicsHandle == nullptr)
    {
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
        
        Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);///Bind the input axis
        Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else{
        UE_LOG(LogTemp, Error, TEXT(" %s Missing input component"), *GetOwner()->GetName())//This gets the name of the object
        //thats missing the component
    }
    

}


void UGrabber::Grab(){
    
    //LINE TRACE and see if we reach any actor why physics body collision channel set
    auto HitResut = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResut.GetComponent(); //Gets the mesh in our case
    auto ActorHit = HitResut.GetActor();
    
    //if we hit something then attach a physics handle
    if(ActorHit)
    {
        
        PhysicsHandle->GrabComponent(
                                 ComponentToGrab,
                                 NAME_None,//No Bones nedded
                                 ComponentToGrab->GetOwner()->GetActorLocation(),
                                 true//Allow rotation
                                 );
    }
    
}

void UGrabber::Release()
{

    PhysicsHandle->ReleaseComponent();//ReleaseComponen
  
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    GetReachLineEnd();
    
    ////if the physics handle attached
    if(PhysicsHandle->GrabbedComponent)
    {
        // move the object thet we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){
    
   
    
    //We are sorting by object type
    //Line trace (AKA Ray-Cast) out to reach distance
    FHitResult HitResult;
    //We have false because we dont want visbility collision rather have player collision, and we have getowner to ignore our self so we can see
    //the object in front of us and not our self
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT HitResult,
                                            GetReachLineStart(),
                                            GetReachLineEnd(),
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );

    return HitResult;
    
}


FVector UGrabber::GetReachLineStart()
{
    
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
           OUT PlayerViewPointLocation,
           OUT PlayerViewPointRotation
                                                               );
    
    
    return PlayerViewPointLocation;
    
    
}

FVector UGrabber::GetReachLineEnd()
{
    
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                OUT PlayerViewPointLocation,
                OUT PlayerViewPointRotation
            );
    
    
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    

}
