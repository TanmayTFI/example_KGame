/**
 * Kanji example - our game class definitions - public domain
 */

/* Include definitions */
#include "KGame.h"

/* One particle */

class Particle : public KObjectListable {
public:
   /* Current center coordinates */
   float x, y;

   /* Velocity over x, y */
   float vx, vy;

   /* Color */
   float fColor[4];

   /* Scale */
   float fScale;

   /* Current age */
   float fAge;

   /* Age increase at each tick */
   float fAgeIncr;
};

/* Our game class definition */

class MyGame : public KGame {
public:
   /** Constructor */
   MyGame ();

   /**
    * Get the name of this game
    *
    * \return game's name
    */
   const char *getGameName (void) { return "Kanjigame example"; }

   /**
    * Get the name of this game, as a safe string that can be used to create folders and files
    *
    * \return safe game's name
    */
   const char *getSafeGameName (void) { return "Example_KGame"; }

   /**
    * Get the name of this game's development studio, as a safe string that can be used to create folders and files
    *
    * \return safe game's developer name
    */
   const char *getSafeDeveloperName (void) { return "Kanji"; }

   /**
    * Get the version of the game that this engine is running
    *
    * \return game's version
    */
   const char *getGameVersion (void) { return "1.0.0"; }

   /**
    * Load settings and create game window. Derive and implement this method
    *
    * \return game window, or NULL to exit
    */
   virtual KWindow *createGameWindow (void);

   /**
    * Run the game. Derive and implement this method
    */
   virtual void run (void);

   /**
    * Close game window and save settings. Derive and implement this method
    */
   virtual void destroyGameWindow (KWindow *_lpKWindow);

private:
   /**
    * Display missing file error
    *
    * \param lpszFileName name of missing file
    */
   static void showFileError (const char *lpszFileName);

   /* Event handler */
   static bool myEventHandler (KEvent *lpKEvent);

   /* UI message handler */
   void myUIMessageHandler (KUIMessage *lpUIMessage);

   /** Current music volume */
   float _fMusicVolume;

   /** Current SFX volume */
   float _fSfxVolume;

   /** Buffer for composing paths to game state files */
   char _szPathBuf[K_MAXPATH];

   /** true if we need to play the 'button clicked' sound effect */
   static bool g_bPlayButtonSound;

   /** List of Particles */
   static KList<Particle> g_lParticles;
};
