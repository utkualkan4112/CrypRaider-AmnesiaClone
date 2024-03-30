// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Actor = GetAcceptableActor();
	if(Actor){
		//UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if(Component != nullptr){	
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetShouldMove(true);
		
	}
	else{
        //UE_LOG(LogTemp, Display, TEXT("Relocking"));
		Mover->SetShouldMove(false);
	}
    
}

AActor* UTriggerComponent::GetAcceptableActor() const {
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for(AActor* Actor : Actors){
		bool HasTag = Actor->ActorHasTag(AcceptableTag);		
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");
		if(HasTag && !IsGrabbed && IsWeighted(Actor)){

			return Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover){
	Mover = NewMover;
}

bool UTriggerComponent::IsWeighted(AActor* Actor) const{
	UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
	Component->SetSimulatePhysics(true);
	if(Component->GetMass() > PressureMass - 1 && Component->GetMass() < PressureMass + 1){
		Component->SetSimulatePhysics(false);
		return true;
	}
	return false;
}



