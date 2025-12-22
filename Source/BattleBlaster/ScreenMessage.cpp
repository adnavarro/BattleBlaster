// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	FText TextMessage = FText::FromString(Message);
	MessageTextBlock->SetText(TextMessage);
}
