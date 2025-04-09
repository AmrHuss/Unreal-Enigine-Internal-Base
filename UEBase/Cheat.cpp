#include "pch.h"





void cheat_manager::cheat_thread()
{
	int x, y; 
	auto& io = ImGui::GetIO();


	SDK::UWorld* gWorld = SDK::UWorld::GetWorld();

	if (!gWorld) return;

	OwningGameInstance = gWorld->OwningGameInstance;
	if (!OwningGameInstance) return;

	LocalPlayer = OwningGameInstance->LocalPlayers[0];
	if (!LocalPlayer) return;

	GameViewportClient = LocalPlayer->ViewportClient;
	if (!GameViewportClient) return;

	PlayerController = LocalPlayer->PlayerController;
	if (!PlayerController) return;

	PlayerController->GetViewportSize(&x, &y);

	MyPlayer = PlayerController->K2_GetPawn();
	if (!MyPlayer) return;

	SDK::TArray<SDK::AActor*> Player;

	UGStatics = (SDK::UGameplayStatics*)SDK::UGameplayStatics::StaticClass();
	if (!UGStatics) return;

	//some games GetAllActorOfClass is STATIC_GetAllActorsOfClass
	UGStatics->GetAllActorsOfClass(gWorld, SDK::AParentCharacter::StaticClass(), &Player);

	//class to math operations
	MathLib = (SDK::UKismetMathLibrary*)SDK::UKismetMathLibrary::StaticClass();
	if (!MathLib) return;

	//iterare through all players
	for (int i = 0; i < Player.Num(); i++)
	{
		if (!Player.IsValidIndex(i)) continue;

		obj = Player[i];
		if (!obj) continue;
		BaseClass = (SDK::AParentCharacter*)obj;
		if (!BaseClass) continue;

		SDK::FVector Location = BaseClass->K2_GetActorLocation();

		if (PlayerController)
			PlayerController->FOV(cfg->fov_changer ? cfg->fov_value : 90); //Simple fov changer

		auto PlayerName = BaseClass->PlayerState->PlayerNamePrivate;
		bool IsVisible = PlayerController->LineOfSightTo(obj, { 0,0,0 }, false); //visible check


		// For
		static bool once = false;
		if (cfg->dump_bones && !once) {
			dump_bones();
			cfg->dump_bones = false;
			once = true;
		}

		//Draw bones
		SDK::FVector2D BoneScreen, PrevBoneScreen;
		

		SDK::FVector HeadLoc = BaseClass->Mesh->GetSocketLocation(StrToName("Head"));
		SDK::FVector RootLoc = BaseClass->Mesh->GetSocketLocation(StrToName("Root")); // if doesnt work either use .contains or dump bones (heavens blessed us with c++23)


		SDK::FVector2D head,Bottom;
		if (PlayerController->ProjectWorldLocationToScreen(RootLoc, &Bottom, false) && PlayerController->ProjectWorldLocationToScreen(HeadLoc, &head, false))
		{
			if (cfg->names)
				ImGui::GetOverlayDrawList()->AddText(ImVec2(head.X,head.Y), IsVisible ? ImColor(0, 255, 0) : ImColor(255, 0, 0), PlayerName.IsValid() ? PlayerName.ToString().c_str() : " ");

			float CornerHeight = abs(head.Y - Bottom.Y);
			float CornerWidth = CornerHeight * 0.6f;
	
			if(cfg->box)
			draw->DrawBox(head.X - (CornerWidth / 2), head.Y, CornerWidth, CornerHeight, IsVisible ? ImColor(0,255,0) : ImColor(255,0,0), 1.0f);
		}

		//example how to draw without bones in relative location 
		SDK::FVector2D Screen;
		if (PlayerController->ProjectWorldLocationToScreen(Location, &Screen, false))
		{
			ImVec2 Pos(Screen.X, Screen.Y);

			if (cfg->lines)
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(static_cast<float>(io.DisplaySize.x / 2), static_cast<float>(io.DisplaySize.y)), Pos, ImColor(255, 255, 255), 0.7);
		}


		// Aimbot Logic from down below adding ssooooooon
		




	}
}

void cheat_manager::dump_bones()
{
	FILE* Log = fopen("C:\\bones.txt", "w");

	if (Log) {
		auto meshname = BaseClass->Mesh->SkeletalMesh->Name;
		auto bonetree = BaseClass->Mesh->SkeletalMesh->Skeleton->BoneTree;
		for (int i = 0; i < bonetree.Num(); i++) {
			auto boneName = BaseClass->Mesh->GetBoneName(i);

			fprintf(Log, "%s = %d,\n", boneName.ToString().c_str(), i);
		}

		fclose(Log); 
		Beep(500, 500);
	}
	else {
		printf("niceeeee\n");
	}
}

