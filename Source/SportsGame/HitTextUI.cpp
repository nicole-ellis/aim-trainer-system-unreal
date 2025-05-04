// Fill out your copyright notice in the Description page of Project Settings.


#include "HitTextUI.h"

void UHitTextUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentLocation = FMath::Lerp(CurrentLocation, TargetLocation, InDeltaTime);
	CurrentOpacity = FMath::Lerp(CurrentOpacity, 0, InDeltaTime);
	HitText->SetOpacity(CurrentOpacity);
	SetPositionInViewport(CurrentLocation);
	if (FVector2D::Distance(CurrentLocation, TargetLocation) < 5)
	{
		RemoveFromParent();
	}
}

void UHitTextUI::NativeConstruct()
{
	Super::NativeConstruct();

	HitText->SetColorAndOpacity(FLinearColor::MakeRandomColor());
}
