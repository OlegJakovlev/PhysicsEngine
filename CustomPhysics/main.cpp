#include "Application/GlutApp.h"

// Test application that uses direct approach instead of DLL calls
int main()
{
	GlutApp* app = GlutApp::Get();

	if (!app->Init("Olegs Jakovlevs", 800, 600))
	{
		return -1;
	}

	app->PostInit();
	app->Start();

	GlutApp::Release();
	return 0;
}
