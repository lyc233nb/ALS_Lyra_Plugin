#include "AlsLinkedAnimationInstance.h"

#include "AlsAnimationInstance.h"
#include "AlsAnimationInstanceProxy.h"
#include "AlsCharacter.h"

#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AlsLinkedAnimationInstance)

class ULyraAnimInstance;

UAlsAnimationInstance* UAlsLinkedAnimationInstance::GetMainAnimInstance() const
{
	return Cast<UAlsAnimationInstance>(GetOwningComponent()->GetAnimInstance());
}

UAlsLinkedAnimationInstance::UAlsLinkedAnimationInstance(): TraceOrigin(), ClippingFrameTime(0.0f), TraceLength(0.0f)
{
	bUseMainInstanceMontageEvaluationData = true;
}

void UAlsLinkedAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Parent = Cast<UAlsAnimationInstance>(GetSkelMeshComponent()->GetAnimInstance());
	Character = Cast<AAlsCharacter>(GetOwningActor());

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld())
	{
		// Use default objects for editor preview.

		if (!Parent.IsValid())
		{
			Parent = GetMutableDefault<UAlsAnimationInstance>();
		}

		if (!IsValid(Character))
		{
			Character = GetMutableDefault<AAlsCharacter>();
		}
	}
#endif
}

void UAlsLinkedAnimationInstance::NativeBeginPlay()
{
	/*ALS_ENSURE_MESSAGE(Parent.IsValid(),
	                   TEXT("%s (%s) should only be used as a linked animation instance within the %s animation blueprint!"),
	                   ALS_GET_TYPE_STRING(UAlsLinkedAnimationInstance).GetData(), *GetClass()->GetName(),
	                   ALS_GET_TYPE_STRING(UAlsAnimationInstance).GetData());*/

	Super::NativeBeginPlay();
}

void UAlsLinkedAnimationInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
}

void UAlsLinkedAnimationInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Character->bIsColliding)
	{
		UpdateWeaponClipping(DeltaSeconds);
	}
	
}

float UAlsLinkedAnimationInstance::GetAimSweepTime()
{
	return UKismetMathLibrary::FClamp(
		GetMainAnimInstance()->AimSweepTimeC,
		0.0f,
		1.0f
		);
}

void UAlsLinkedAnimationInstance::UpdateWeaponClipping(float DeltaTime)
{
	if (!Character)
	{
		
		Character=GetALSCharacter();
		
	}
	else
	{
		if (!Character->bIsColliding)
			return;


		if (!CurveData) return;
		//Trace origin setting

		

		TraceOrigin=Character->GetMesh()->GetSocketTransform("neck_01").GetLocation();

			//z axis value needed

		if (Character->GetRotationMode()==AlsRotationModeTags::ViewDirection||
			Character->GetRotationMode()==AlsRotationModeTags::VelocityDirection)
		{
			ClippingFrameTime=UKismetMathLibrary::FInterpTo(
            			ClippingFrameTime,
            			0.0f,
            			DeltaTime,
            			ClippingFrameTimeSpeed
            			);
			return;
		}
		float OutValue=0.0f;
		
		TraceLength=
			CurveData->GetVectorValue(0.0f).Z;
		GetCurveValueWithDefault(FName("BasePose_Cover"),0.0f,OutValue);
		float CoverMaxClipping=
			UKismetMathLibrary::MapRangeClamped(OutValue,0.0f,1.0f,1.0f,0.7f);
		float TraceLengthScaled=
			CurveData->GetVectorValue(GetAimSweepTime()).X;
		float OutAlpha=0.0f;
		float InRangeA=
			UKismetMathLibrary::Lerp(35.0f,15.0f,GetCurveValueWithDefault(FName("BasePose_CLF"),0.0f,OutAlpha));
		float OutRangeA=
			CurveData->GetVectorValue(GetAimSweepTime()).X*CoverMaxClipping;

		FVector TraceStart=
			UKismetMathLibrary::GetRightVector(
				Character->GetControlRotation())*5.0f
			+TraceOrigin
			+UKismetMathLibrary::GetForwardVector(
			Character->GetControlRotation())*10.0f;
		FVector TraceEnd=
			UKismetMathLibrary::GetRightVector(
				Character->GetControlRotation())*5.0f
			+TraceOrigin
			+UKismetMathLibrary::GetForwardVector(
		Character->GetControlRotation())*TraceLength*TraceLengthScaled;
		TArray<AActor*>ActorsToIgnore={Character,
							UAlsUtility::GetWeaponActor(Character)};
		
		if (UKismetSystemLibrary::SphereTraceSingle(
				GetWorld(),
				TraceStart,
				TraceEnd,
				2.0f,
				static_cast<ETraceTypeQuery>(ECC_Visibility),
				false,
				ActorsToIgnore,
				EDrawDebugTrace::Type::ForOneFrame,
				HitResult,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0f
			))
		{
			ClippingFrameTime=
				UKismetMathLibrary::FInterpTo(
					ClippingFrameTime,
					UKismetMathLibrary::MapRangeClamped(
						UKismetMathLibrary::Vector_Distance(
							HitResult.Location,
							HitResult.TraceStart
							)
						,
						InRangeA,
						TraceLength,
						OutRangeA,
						0.0f
						),
						DeltaTime,
						10.0f
					);
		}
		else
		{
			ClippingFrameTime=UKismetMathLibrary::FInterpTo(
				ClippingFrameTime,
				0.0f,
				DeltaTime,
				10.0f
				);
		}
	}
	
}

FAnimInstanceProxy* UAlsLinkedAnimationInstance::CreateAnimInstanceProxy()
{
	return new FAlsAnimationInstanceProxy{this};
}

void UAlsLinkedAnimationInstance::ReinitializeLook()
{
	if (Parent.IsValid())
	{
		Parent->ReinitializeLook();
	}
}

void UAlsLinkedAnimationInstance::RefreshLook()
{
	if (Parent.IsValid())
	{
		Parent->RefreshLook();
	}
}

void UAlsLinkedAnimationInstance::ResetGroundedEntryMode()
{
	if (Parent.IsValid())
	{
		Parent->ResetGroundedEntryMode();
	}
}

void UAlsLinkedAnimationInstance::SetHipsDirection(const EAlsHipsDirection NewHipsDirection)
{
	if (Parent.IsValid())
	{
		Parent->SetHipsDirection(NewHipsDirection);
	}
}

void UAlsLinkedAnimationInstance::ActivatePivot()
{
	if (Parent.IsValid())
	{
		Parent->ActivatePivot();
	}
}

void UAlsLinkedAnimationInstance::ResetJumped()
{
	if (Parent.IsValid())
	{
		Parent->ResetJumped();
	}
}

AAlsCharacter* UAlsLinkedAnimationInstance::GetALSCharacter() const
{
	return Cast<AAlsCharacter>(TryGetPawnOwner());
}
