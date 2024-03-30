// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FRotator CurrentRoation = GetOwner()->GetActorRotation();
	//FString RotationString = CurrentRoation.ToCompactString();

	//UE_LOG(LogTemp, Display, TEXT("%s"), *RotationString);

	//UWorld* World = GetWorld();

	//float time = World->TimeSeconds;
	//UE_LOG(LogTemp, Display, TEXT("time is: %f"), time);

	UPhysicsHandleComponent* PhysicsHandle = GetPyhsicsHandle();

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent()){
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * MoveDistance;
	    PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
}

void UGrabber::Release(){
	UPhysicsHandleComponent* PhysicsHandle = GetPyhsicsHandle();
	
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent()){
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
	UE_LOG(LogTemp, Display, TEXT("Release"));
	
}

void UGrabber::Grab(){
	UPhysicsHandleComponent* PhysicsHandle = GetPyhsicsHandle();
	if(PhysicsHandle == nullptr){
		return;
	}
	FHitResult HitResult;
	
	if(GetGrabbableInReach(HitResult)){
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Purple, false, 5);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);

		//UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetActorNameOrLabel());
	}
	
}

UPhysicsHandleComponent* UGrabber::GetPyhsicsHandle() const{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr){
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent not found"));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult &HitResult){
	FVector Start = GetComponentLocation();
	FVector End = MaxGrabDistance * GetForwardVector() + Start; 
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	//DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(HitResult,
	 Start, End,
	 FQuat::Identity,
	 ECC_GameTraceChannel2,
	 Sphere);
	 
}

