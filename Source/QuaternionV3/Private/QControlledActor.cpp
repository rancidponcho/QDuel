#include "QControlledActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AQControlledActor::AQControlledActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMesh;
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetEnableGravity(false);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 3000.0f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(90.0f);

	// PID
	Kp = 2500000.0;
	Ki = 0.0;
	Kd = 0.0;
}

// Called when the game starts or when spawned
void AQControlledActor::BeginPlay()
{
	Super::BeginPlay();
	


}

// Called every frame
void AQControlledActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// Get the actor's location
	FVector ActorLocation = GetActorLocation();
	//// Get the forward vector of the camera and the forward vector of the skeletal mesh
	FVector TargetDirection = Camera->GetForwardVector();

	DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + TargetDirection * 1000, FColor::Green, false, 1, 0, 5);
	// FVector CurrentDirection = -(SkeletalMesh->GetForwardVector());
	//// Calculate the end points of the debug lines
	//FVector EndPointTarget = (ActorLocation + TargetDirection) * 1000; // Assuming a length of 100 for the debug line
	//FVector EndPointCurrent = (ActorLocation + CurrentDirection) * 1000; // Assuming a length of 100 for the debug line
	//// Draw debug lines from the actor's location to the end points of the vectors
	//DrawDebugLine(GetWorld(), ActorLocation, EndPointTarget, FColor::Green, false, -1, 0, 5);
	//DrawDebugLine(GetWorld(), ActorLocation, EndPointCurrent, FColor::Red, false, -1, 0, 5);


	//DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + TargetDirection * 5000, FColor::Green, false, -1, 0, 5);
}

void AQControlledActor::Look(const FVector2D Direction)
{
	const float MouseSensitivity = 1.0f; // TODO: make settings :)

	// Calculate the rotation delta based on mouse input
	const float YawChange = Direction.X * MouseSensitivity;
	const float PitchChange = Direction.Y * MouseSensitivity;

	// Convert the rotation changes to quaternions
	FQuat YawQuat = FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(YawChange));
	FQuat PitchQuat = FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(PitchChange));

	// Apply the rotation changes to the camera's transform
	FTransform Transform = FTransform();
	Transform = SpringArm->GetComponentTransform();
	Transform.ConcatenateRotation(YawQuat);
	Transform.ConcatenateRotation(PitchQuat);
	Transform.NormalizeRotation();

	// yay no gimbal lock
	SpringArm->SetWorldTransform(Transform);
}

void AQControlledActor::PointAt(const bool isPointing)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	// Get the actor's location
	FVector ShipForward = -SkeletalMesh->GetForwardVector();
	FVector CameraForward = Camera->GetForwardVector();

	// Calculate the error (angle between the ship's forward vector and the camera's forward vector)
	float Angle = FMath::Acos(FVector::DotProduct(ShipForward.GetSafeNormal(), CameraForward.GetSafeNormal()));

	// Integral
	IntegralError += Angle * DeltaTime; // Accumulate the error over time
	
	// Calculate the angular velocity (change in angle over time)
	float AngularVelocity = (Angle - PrevAngle) / DeltaTime;
	PrevAngle = Angle;

	// Calculate the torque needed to rotate the ship towards the camera
	FVector TorqueVector = FVector::CrossProduct(ShipForward, CameraForward) * (Kp * Angle + Kd * AngularVelocity + Ki * IntegralError);

	// Apply torque
	SkeletalMesh->AddTorqueInRadians(TorqueVector);
}

void AQControlledActor::MoveXYZ(const FVector Direction)
{
	FVector LocalDirection = GetActorTransform().TransformVector(Direction.GetSafeNormal());
	const float ImpulseMagnitude = 1000.0f; // TODO: Make member variable
	FVector Impulse = LocalDirection * ImpulseMagnitude;
	SkeletalMesh->AddImpulse(Impulse);
}

void AQControlledActor::Roll(const float Direction)
{
	FVector LocalRollAxis = GetActorTransform().GetRotation().GetForwardVector();
	float RollTorqueMagnitude = 10000000.0f;			// Example torque magnitude TODO: Make member variable
	float LocalRollTorque = Direction * RollTorqueMagnitude;
	FVector RollTorque = LocalRollAxis * LocalRollTorque;
	SkeletalMesh->AddTorqueInRadians(RollTorque);
} 

