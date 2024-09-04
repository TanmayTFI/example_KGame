/**
 * Kanji example - public domain
 */

#include "KPTK.h"
#include "MyGame.h"
/* Windows uses WinMain(). Mac, iPhone and iPad use kanjiMain() */

#ifdef K_WIN32
INT WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, INT iCmdShow) {
#else
int kanjiMain (int argc, char **argv) {
#endif
   MyGame *lpGame;

   /* Create, execute and release game */

   lpGame = new MyGame;
   lpGame->main ();
   delete lpGame;

   return 0;
}
