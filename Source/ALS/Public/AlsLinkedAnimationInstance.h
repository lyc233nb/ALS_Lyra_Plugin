#pragma once

#include "Animation/AnimInstance.h"
#include "AlsLinkedAnimationInstance.generated.h"

class ULyraAnimInstance;
class UAlsAnimationInstance;
class UCurveVector;
class AAlsCharacter;

UCLASS()
class ALS_API UAlsLinkedAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UAlsAnimationInstance> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<AAlsCharacter> Character;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="WeaponClipping")
	TObjectPtr<UCurveVector> CurveData;

	UPROPERTY(BlueprintReadWrite)
	FHitResult HitResult;

	UFUNCTION(BlueprintCallable,meta=(BlueprintThreadSafe))
	UAlsAnimationInstance* GetMainAnimInstance() const;

	UPROPERTY(BlueprintReadWrite)
	FVector TraceOrigin;

public:
	UAlsLinkedAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable,meta=(BlueprintThreadSafe))
	float GetAimSweepTime();
	
	UFUNCTION(BlueprintCallable,meta=(BlueprintThreadSafe))
	void UpdateWeaponClipping(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = "WeaponClipping")
	float ClippingFrameTime;

	UPROPERTY(BlueprintReadWrite,Category="WeaponClipping")
	float TraceLength;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	UPROPERTY(EditAnywhere)
	float ClippingFrameTimeSpeed=5.0f;

protected:
	// Be very careful when using this function to read your custom variables using the property access system. It is
	// safe to use this function to read variables that change only inside UAlsAnimationInstance::NativeUpdateAnimation()
	// because it is guaranteed that this function will be called before parallel animation evaluation. Reading variables
	// that change in other functions can be dangerous because they can be changed in the game thread at the same
	// time as being read in the worker thread, which can lead to undefined behavior or even a crash. If you're not
	// sure what you're doing, then it's better to access your custom variables through the "Parent" variable.
	UFUNCTION(BlueprintPure, Category = "ALS|Linked Animation Instance",
		Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Parent"))
	UAlsAnimationInstance* GetParentUnsafe() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ReinitializeLook();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void RefreshLook();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetGroundedEntryMode();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void SetHipsDirection(EAlsHipsDirection NewHipsDirection);

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ActivatePivot();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	void ResetJumped();

	UFUNCTION(meta=(BlueprintThreadSafe))
	inline AAlsCharacter* GetALSCharacter() const;
	
};

inline UAlsAnimationInstance* UAlsLinkedAnimationInstance::GetParentUnsafe() const
{
	return Parent.Get();
}
