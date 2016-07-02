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
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
    //This keeps the variable private but visiable in the unreal engine
    UPROPERTY(VisibleAnywhere)
    float OpenAngle = 90.0f;
    
    //A trigger volume, is a invisvible volume in the game world that can be used to tell code to do something 
    UPROPERTY(EditAnywhere)
    ATriggerVolume  *Pressureplate;
    
	
};
