#include "pch.h"

//engine stuff meow

class cheat_manager {
private:
	SDK::UWorld** _UWorld;
	SDK::UWorld* gWorld;
	SDK::APlayerController* PlayerController;
	SDK::ULocalPlayer* LocalPlayer;
	SDK::UGameInstance* OwningGameInstance;
	SDK::UGameViewportClient* GameViewportClient;
	SDK::AGameStateBase* GameState;
	SDK::AActor* obj;
	SDK::UGameplayStatics* UGStatics;
	SDK::UKismetSystemLibrary* KismetSystemLib;
	SDK::APawn* MyPlayer;
	SDK::AParentCharacter* BaseClass; //change this class for each game
	SDK::UKismetMathLibrary* MathLib;


	//aimbot stuff

	SDK::FVector head_bone_pos;
	SDK::FVector feet_bone_pos;
	SDK::FVector2D Bottom{}, Top{};
	SDK::FVector2D screen_middle;
	SDK::FVector CameraLocation;
	SDK::FRotator rot;
	SDK::FRotator closest_actor_rotation;
	SDK::FVector closest_actor_head;
	SDK::AActor* closest_actor;
	SDK::FVector2D screen_center;
	SDK::FVector2D screen_size; //use x y from the GetViewportSize 
	SDK::FVector2D screen_middle_pos;
	SDK::FVector2D screen_pos;


public:
	void cheat_thread();
	void dump_bones();
};


//Bullshit engine  so have to use some helper funcs
inline SDK::FName StrToName(std::string str)
{
	std::wstring wstr(str.begin(), str.end());
	const wchar_t* wstr_cstr = wstr.c_str();

	return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(TEXT(wstr_cstr)));
}


inline SDK::FName StrCToName(const wchar_t* str)
{
	return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(TEXT(str)));
}



// For aimbot smoothing helper function
void SmoothAim(SDK::FRotator currentRotation, SDK::FRotator targetRotation, float smoothFactor)
{
	const float deltaTime = 1.0f / 60.0f;
	SDK::FRotator smoothedRotation = SDK::UKismetMathLibrary::RInterpTo(currentRotation, targetRotation, deltaTime, smoothFactor);

	MyController->SetControlRotation(smoothedRotation);
}

