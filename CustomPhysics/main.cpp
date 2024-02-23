#include "Application/GlutApp.h"

// Test application that uses direct approach instead of DLL calls
int main()
{
	CustomApplication::GlutApp* app = CustomApplication::GlutApp::Get();

	if (!app->Init("Olegs Jakovlevs", 800, 600))
	{
		return -1;
	}

	app->PostInit();
	app->Start();

	return 0;
}
