#include <windows.h>
#include "game/Game.h"

// TODO:
// 
// Housekeeping
// -- Get rid of warnings
// -- clean & dry
// -- Change game from class to function
// 
// Most CPU intensive functions:
// -- CollisionGrid::CheckCollision
// -- Not that hot, but recreating view matrix every frame is ridiculous
//  
// Level Editing Tools
// -- Will be easier to add once collision is all set
// -- At simplest: some functions to put rails and stuff in
// -- At most complex: full graphical level editor
// 
// Render Optimizations
// -- Find a good way to only render whats in sight
// -- Maybe just check intersection with camera on every render
// ---- Is this slower than just rendering?
// -- Ideally, I could have one spatial data structure to use for rendering and collision
// -- Another note: maybe implement the tiling outlined in the comments of default.frag
// 
// 
// Manual (skate trick)
// -- Figure out how exactly a manual input should be defined
// -- For this (and maybe kickflip) maybe make some sort of input function like:
// -- GetAxisVelocity(int axis) where it measures the change in input, and then
// -- if its high enough (a flick) register the trick
//


#ifdef _DEBUG

int main(void)
{
	Game game("Skateboard Baby!");
	return game.Run();
}

#endif 

#ifdef NDEBUG

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPSTR     lpCmdLine,
			_In_ int       nShowCmd) 
{

	Game game("Skateboard Baby!");
	return game.Run();
}

#endif 