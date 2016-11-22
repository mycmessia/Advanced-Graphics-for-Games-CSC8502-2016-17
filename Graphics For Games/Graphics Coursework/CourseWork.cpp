#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "CourseWorkRenderer.h"

int main ()
{
	Window w ("MYC's CourseWork Of CSC8502!", 800, 600, false);
	if (!w.HasInitialised ())
	{
		return -1;
	}

	Renderer renderer (w);
	if (!renderer.HasInitialised ())
	{
		return -1;
	}

	w.LockMouseToWindow (true);
	w.ShowOSPointer (false);

	while (w.UpdateWindow () && !Window::GetKeyboard ()->KeyDown (KEYBOARD_ESCAPE))
	{
		renderer.UpdateScene (w.GetTimer ()->GetTimedMS ());
		renderer.CalcFPS (w.GetTimer ()->GetTimedMS ());
		renderer.RenderScene ();
	}

	return 0;
}