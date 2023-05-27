#include <windows.h>
#include "game/Game.h"

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