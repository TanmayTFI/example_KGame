/**
 * Kanji example - public domain
 */

#include "KPTK.h"
#include "KSound.h"
#include "KVideo.h"
#include "MyGame.h"

/** Constructor */

MyGame::MyGame () :
      _fMusicVolume (1.0f),
      _fSfxVolume (1.0f) {
}

/**
 * Load settings and create game window. Derive and implement this method
 *
 * \return game window, or NULL to exit
 */

KWindow *MyGame::createGameWindow (void) {
   KWindow *lpKWindow;
   KIniReader *lpIniReader;
   bool bSuccess;

   /* Add pakfile */
   KResource::addArchive ("resources.mp3", K_RESARCHIVE_REQUIRED);

   /* Start log */
   KStringFmt (_szPathBuf, K_MAXPATH, "%s/log.txt", getStateFolder());
   KPTK::enableLog (KMiscTools::makeFilePath (_szPathBuf), "example", "1.0", true);

   /* Read settings, if they exist */
   KStringFmt (_szPathBuf, K_MAXPATH, "%s/settings.ini", getStateFolder());
   lpIniReader = new KIniReader;
   bSuccess = lpIniReader->setIni (KMiscTools::makeFilePath (_szPathBuf));
   if (bSuccess) {
      _fMusicVolume = lpIniReader->getFloatValue ("settings", "music", 1.0f);
      _fSfxVolume = lpIniReader->getFloatValue ("settings", "sfx", 1.0f);
   }
   delete lpIniReader;

   /* Create game window */
   lpKWindow = KPTK::createKWindow (K_OPENGL, true);
   lpKWindow->createGameWindow (800, 600, 32, false, "Kanjigame example");
   lpKWindow->setPTKCallBack (myEventHandler);
   
   /* Use software mouse cursor */
   KInput::hidePointer ();
   lpKWindow->displayMouse (true);

   return lpKWindow;
}

/**
 * Display missing file error
 *
 * \param lpszFileName name of missing file
 */

void MyGame::showFileError (const char *lpszFileName) {
   static char szFileMsg[K_MAXPATH];

   KStringFmt (szFileMsg, K_MAXPATH, KStr ("ERR_BODY"), lpszFileName);
   KMiscTools::messageBox (KStr ("ERR_TITLE"), szFileMsg);
}

/**
 * Run the game. Derive and implement this method
 */

void MyGame::run (void) {
   KGraphic *lpBgGraphic, *lpParticleGraphic, *lpWindowGraphic, *lpButtonGraphic;
   KBatch *lpBatch;
   KSound *lpLaserSnd, *lpButtonSnd;
   KTrueText *lpFont;
   KUIElement *lpUIContainer, *lpUISpacer;
   KUIImage *lpUIOptionsWindow;
   KUILayout *lpUIOptionsLayout, *lpUILayout;
   KUISelector *lpUISelector;
   KUIText *lpUIText;
   KUISlider *lpUIMusicSlider, *lpUISfxSlider;
   KUIButton *lpUIOptionsButton, *lpUI3dModelButton, *lpUIFsButton, *lpUIOkButton;
   KModel *lpModel;
   long i, nLastModelMx = 400, nLastModelMy = 300;
   bool bSuccess, bMouseWasDown = false, bShow3dModel = false;
   const char *lpszCaption;

   /* Catch the 'back' key */
   KSysAndroid::catchBackKey (true);
   
   /* Cancel local notifications */
   KSysAndroid::cancelLocalNotifications();
   
   /* Load strings table */
   readStringTable (KMiscTools::makeFilePath ("english.txt"));
   lpszCaption = KStr ("CAPTION");

   /* Load background */
   lpBgGraphic = getGraphics()->loadGraphic (KMiscTools::makeFilePath ("flower.jpg"), true, true);
   if (!lpBgGraphic) {
      showFileError ("flower.jpg");
      return;
   }
   lpBgGraphic->setTextureQuality (true);

   /* Load particle */
   lpParticleGraphic = getGraphics()->loadGraphic (KMiscTools::makeFilePath ("particle.png"), true, true);
   if (!lpParticleGraphic) {
      showFileError ("particle.png");
      return;
   }
   lpParticleGraphic->setTextureQuality (true);

   /* Load options background */
   lpWindowGraphic = getGraphics()->loadGraphic (KMiscTools::makeFilePath ("window.png"), true, true);
   if (!lpWindowGraphic) {
      showFileError ("window.png");
      return;
   }
   lpWindowGraphic->setTextureQuality (true);

   /* Load button graphics */
   lpButtonGraphic = getGraphics()->loadGraphic (KMiscTools::makeFilePath ("buttons.png"), true, true);
   if (!lpButtonGraphic) {
      showFileError ("buttons.png");
      return;
   }
   lpButtonGraphic->setTextureQuality (true);

   /* Upload graphics to GPU, packing them into one or more atlases */
   getGraphics()->beginAtlas (1024, 1024, 800, 600);
   getGraphics()->uploadGraphic (lpButtonGraphic);
   getGraphics()->uploadGraphic (lpWindowGraphic);
   getGraphics()->uploadGraphic (lpParticleGraphic);
   getGraphics()->uploadGraphic (lpBgGraphic);
   getGraphics()->endAtlas ();
   
   /* Load font */
   lpFont = new KTrueText (KMiscTools::makeFilePath ("acmesa.ttf"));
   lpFont->setHeightPix (20);

   /* Initialize UI and create container for all the elements */

   KUIElement::initialize (_lpKWindow);

   lpUIContainer = new KUIElement (KUIElement::getRootElement());
   lpUIContainer->setPosition (0, 0);
   lpUIContainer->setSize (800, 600);
   lpUIContainer->setMessageFunction (K_UIMSG_FUNCTION (MyGame, myUIMessageHandler, this));

   /* Create options button */

   lpUIOptionsButton = new KUIButton (lpUIContainer);
   lpUIOptionsButton->setPosition (800 - 204, 600 - 91 + 10);
   lpUIOptionsButton->setSize (204, 91);
   lpUIOptionsButton->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 376, 0, 580, 91);
   lpUIOptionsButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 376, 0, 580, 91);
   lpUIOptionsButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 376, 95, 580, 95 + 91);
   lpUIOptionsButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 376, 190, 580, 190 + 91);
   lpUIOptionsButton->setTextFont (lpFont);
   lpUIOptionsButton->setTextOutlineThickness (2);
   lpUIOptionsButton->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIOptionsButton->setText (KStr ("UI_OPTIONS"));

   /* Create "3d model" button */
   
   lpUI3dModelButton = new KUIButton (lpUIContainer);
   lpUI3dModelButton->setPosition (800 - 204, 600 - 2 * 91 + 10);
   lpUI3dModelButton->setSize (204, 91);
   lpUI3dModelButton->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 376, 0, 580, 91);
   lpUI3dModelButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 376, 0, 580, 91);
   lpUI3dModelButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 376, 95, 580, 95 + 91);
   lpUI3dModelButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 376, 190, 580, 190 + 91);
   lpUI3dModelButton->setTextFont (lpFont);
   lpUI3dModelButton->setTextOutlineThickness (2);
   lpUI3dModelButton->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUI3dModelButton->setText (KStr ("UI_3DOBJECT"));
   
   /* Create options window and its contents */
   
   lpUIOptionsWindow = new KUIImage (lpUIContainer);
   lpUIOptionsWindow->setPosition (floorf ((800 - lpWindowGraphic->getWidth()) / 2), floorf ((600 - lpWindowGraphic->getHeight()) / 2));
   lpUIOptionsWindow->setSize (lpWindowGraphic->getWidth(), lpWindowGraphic->getHeight ());
   lpUIOptionsWindow->setGraphic (lpWindowGraphic, 0, 0, lpWindowGraphic->getWidth(), lpWindowGraphic->getHeight ());
   lpUIOptionsWindow->setBlend (0); /* Start out as hidden */

   lpUIOptionsLayout = new KUILayout (lpUIOptionsWindow);
   lpUIOptionsLayout->setPosition (0, 70);
   lpUIOptionsLayout->setSize (lpUIOptionsWindow->getWidth(), lpUIOptionsWindow->getHeight () - 70 - 76);
   lpUIOptionsLayout->setLayoutType (K_UILAYOUT_VERTICAL);
   lpUIOptionsLayout->setHorizAlignMode (K_UIELEMENT_HCENTER);
   lpUIOptionsLayout->setVSpacing (0);
   
   lpUILayout = new KUILayout (lpUIOptionsLayout);
   lpUILayout->setSize (lpUIOptionsWindow->getWidth(), (float) lpFont->getHeightPix() + 2 + 91);
   lpUILayout->setLayoutType (K_UILAYOUT_VERTICAL);
   lpUILayout->setHorizAlignMode (K_UIELEMENT_HCENTER);

   lpUIText = new KUIText (lpUILayout);
   lpUIText->setSize (lpFont->getStringWidth (KStr ("UI_MUSICVOL")), (float) lpFont->getHeightPix());
   lpUIText->setTextFont (lpFont);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setText (KStr ("UI_MUSICVOL"));

   lpUIMusicSlider = new KUISlider (lpUILayout);
   lpUIMusicSlider->setSize (352, 91);
   lpUIMusicSlider->setActiveRect (true, 34, 20, 319, 71);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 0, 285, 352, 285 + 91);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 285, 352, 285 + 91);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 285, 352, 285 + 91);
   lpUIMusicSlider->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 285, 352, 285 + 91);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 363, 309, 415, 360);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 363, 309, 415, 360);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 363, 384, 415, 435);
   lpUIMusicSlider->setButtonGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 363.5f, 449, 415.5f, 500);
   lpUIMusicSlider->setValue (_fMusicVolume);
   (void) lpUIMusicSlider->hasValueChanged ();
   lpUIMusicSlider->setName ("music_slider");   /* Just for the message handler to know not to play a sound when clicked */

   lpUILayout = new KUILayout (lpUIOptionsLayout);
   lpUILayout->setSize (lpUIOptionsWindow->getWidth(), (float) lpFont->getHeightPix() + 2 + 91);
   lpUILayout->setLayoutType (K_UILAYOUT_VERTICAL);
   lpUILayout->setHorizAlignMode (K_UIELEMENT_HCENTER);

   lpUIText = new KUIText (lpUILayout);
   lpUIText->setSize (lpFont->getStringWidth (KStr ("UI_SFXVOL")), (float) lpFont->getHeightPix());
   lpUIText->setTextFont (lpFont);
   lpUIText->setTextOutlineThickness (2);
   lpUIText->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIText->setText (KStr ("UI_SFXVOL"));

   lpUISfxSlider = new KUISlider (lpUILayout);
   lpUISfxSlider->copyFromTemplate (lpUIMusicSlider);
   lpUISfxSlider->setValue (_fSfxVolume);
   (void) lpUISfxSlider->hasValueChanged ();

   lpUISpacer = new KUIElement (lpUIOptionsLayout);
   lpUISpacer->setSize (lpUIOptionsLayout->getWidth(), 80);
   
   lpUIOkButton = new KUIButton (lpUIOptionsLayout);
   lpUIOkButton->setSize (352, 91);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_DISABLED, lpButtonGraphic, 0, 0, 352, 91);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_ENABLED, lpButtonGraphic, 0, 0, 352, 91);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_MOUSEOVER, lpButtonGraphic, 0, 95, 352, 95 + 91);
   lpUIOkButton->setBackgroundGraphic (K_UIELEMENT_DOWN, lpButtonGraphic, 0, 190, 352, 190 + 91);
   lpUIOkButton->setTextFont (lpFont);
   lpUIOkButton->setTextOutlineThickness (2);
   lpUIOkButton->setTextOutlineColor (0, 0, 0, 0.125f);
   lpUIOkButton->setText (KStr ("UI_OK"));

   /* Create vertex buffer */
   lpBatch = KPTK::createKBatch ();
   lpBatch->allocateBuffer (500);

   /* Load 3d model and its textures */
   
   lpModel = new KModel;
   bSuccess = lpModel->loadModel (KMiscTools::makeFilePath ("kanji_1.3ds"));
   if (!bSuccess) {
      showFileError ("kanji_1.3ds");
      return;
   }
   bSuccess = lpModel->loadTextures (KMiscTools::makeFilePath (""));
   if (!bSuccess) {
      KMiscTools::messageBox (KStr ("ERR_TITLE"), KStr ("ERR_3DTEXTURES"));
      return;
   }
   
   /* Play music using the Android device hardware */
   KMiscTools::playBackgroundMusic (KMiscTools::makeFilePath ("juniorgroove.mp3"), 100, true, false);

   /* Load laser sound effect */
   lpLaserSnd = new KSound;
   bSuccess = lpLaserSnd->loadSample (KMiscTools::makeFilePath ("laser.wav"), 100, false, 4);
   if (!bSuccess) {
      showFileError ("laser.wav");
      return;
   }

   /* Load button sound effect */
   lpButtonSnd = new KSound;
   bSuccess = lpButtonSnd->loadSample (KMiscTools::makeFilePath ("click.wav"), 100, false, 4);
   if (!bSuccess) {
      showFileError ("click.wav");
      return;
   }

   /* Render loop */

   resetFrameLoop ();

   do {
      Particle *p;
      bool bGotNext;
      float x, y;
      long nTicks;

      beginFrame ();
      KUIElement::moveAll ();

      if (!KUIElement::getMousedOverElement() && lpUIOptionsButton->getBlend() > 0 && KInput::getLeftButtonState()) {
         long mx = KInput::getMouseX ();
         long my = KInput::getMouseY ();

         /* Mouse is down, not over a UI element, and options panel isn't shown; insert a few particles */
         for (i = 0; i < 10; i++) {
            Particle *p = new Particle;

            p->x = mx + (KRandom::getRandomFloat() - 0.5f) * 3;
            p->y = my + (KRandom::getRandomFloat() - 0.5f) * 3;
            p->vx = (KRandom::getRandomFloat() - 0.5f) * 4.0f;
            p->vy = (KRandom::getRandomFloat() - 0.5f) * 4.0f;
            p->fScale = KRandom::getRandomFloat() * 0.4f + 0.25f;
            p->fColor[0] = 1;
            p->fColor[1] = 0.5f + KRandom::getRandomFloat() * 0.25f;
            p->fColor[2] = 0.15f + KRandom::getRandomFloat() * 0.1f;
            p->fColor[3] = 0.5f + KRandom::getRandomFloat() * 0.25f;
            p->fAge = 0;
            p->fAgeIncr = 0.01f + KRandom::getRandomFloat() * 0.01f;

            g_lParticles.addToTail (p);
         }

         if (!bMouseWasDown) {
            lpLaserSnd->playSample ();
            bMouseWasDown = true;
         }
      }
      else 
         bMouseWasDown = false;

      /* Check UI */
      if (lpUIOptionsButton->isClicked ()) {
         /* Show the options panel */
         lpUIOptionsWindow->animateScale (0.1f, 1.0f, 0, 250, K_COUNTER_EASEOUT);
         lpUIOptionsWindow->animateZAngle (20, 0, 0, 250, K_COUNTER_EASEOUT);
         lpUIOptionsWindow->animatePosition (420, 320, floorf ((800 - lpWindowGraphic->getWidth()) / 2), floorf ((600 - lpWindowGraphic->getHeight()) / 2), 0, 250, K_COUNTER_EASEOUT);
         lpUIOptionsWindow->animateBlend (0, 1, 0, 250);
         lpUIOptionsButton->animateBlend (1, 0, 0, 250);
      }
      if (lpUI3dModelButton->isClicked ()) {
         /* Show/hide 3d object */
         bShow3dModel = (bShow3dModel) ? false : true;
      }
      if (lpUIOkButton->isClicked ()) {
         /* Hide the options panel */
         lpUIOptionsWindow->animateBlend (1, 0, 0, 250);
         lpUIOptionsWindow->animateScale (1.0f, 0.1f, 0, 250, K_COUNTER_EASEIN);
         lpUIOptionsWindow->animatePosition (floorf ((800 - lpWindowGraphic->getWidth()) / 2), floorf ((600 - lpWindowGraphic->getHeight()) / 2), 420, 320, 0, 250, K_COUNTER_EASEIN);
         lpUIOptionsWindow->animateZAngle (0, -20, 0, 250, K_COUNTER_EASEIN);
         lpUIOptionsButton->animateBlend (0, 1, 0, 250);
      }
      if (lpUIMusicSlider->hasValueChanged () || lpUISfxSlider->hasValueChanged ()) {
         /* Update music and SFX volume */
         _fMusicVolume = lpUIMusicSlider->getValue ();
         _fSfxVolume = lpUISfxSlider->getValue ();
         KSound::setGlobalVolumes ((long) (100 * _fMusicVolume), (long) (100 * _fSfxVolume));
      }
 
      if (g_bPlayButtonSound) {
         /* Play 'button clicked' sound effect */
         lpButtonSnd->playSample ();
         g_bPlayButtonSound = false;
      }

      /* Update logic in ticks of 10 milliseconds */
      nTicks = getTickCount ();
      while (nTicks > 0) {
         /* Update particles */
         bGotNext = g_lParticles.getHead (p);
         while (bGotNext) {
            Particle *lpNextParticle = p;
            
            bGotNext = g_lParticles.getNext (lpNextParticle);
            
            p->x += p->vx;
            p->y += p->vy;
            p->fAge += p->fAgeIncr;

            if (p->fAge >= 1) {
               g_lParticles.remove (p);
               delete p;
            }

            p = lpNextParticle;
         }
         
         nTicks--;
      }

      /* Draw background */

      _lpKWindow->setWorldView (0, 0, 0.0f, 1.0f, false);      
      lpBgGraphic->blitRectF (0, 0, 800, 600, 0, 0, false, false);

      /* Draw all particles */

      lpParticleGraphic->setAlphaMode (0);   /* Additive alpha */
      lpBatch->beginBatch (lpParticleGraphic);
      bGotNext = g_lParticles.getHead (p);
      while (bGotNext) {
         lpParticleGraphic->setBlitColor (p->fColor[0], p->fColor[1], p->fColor[2], p->fColor[3]); 
         lpBatch->blitAlphaRectFx (0, 0, 64, 64, p->x - 32, p->y - 32, 0.0f, p->fScale, 1 - p->fAge, false, false);
         bGotNext = g_lParticles.getNext (p);
      }
      lpBatch->endBatch ();

      if (bShow3dModel) {
         KStage stage (_lpKWindow);
         KVector3 vEye = KVector3 (0, 0, -600), vTarget = KVector3 (0, 0, 0), vUp = KVector3 (0, 1, 0);
         KMatrix matProjection, matCamera, matWorld, mat;
         
         /* Draw 3d object - set projection and camera matrices, and world matrix for the object */
         
         matProjection.perspective (40.0f * K_PI_F / 180.0f, 800.0f / 600.0f, 1, 5000);         
         matCamera.camera (vEye, vTarget, vUp);
         
         if (lpUIOptionsWindow->getBlend() <= 0) {
            nLastModelMx = KInput::getMouseX();
            nLastModelMy = KInput::getMouseY();
         }
         
         matWorld.rotationY ((nLastModelMx - 400) * K_PI_F / nLastModelMx);
         mat.rotationX ((nLastModelMy - 300) * K_PI_F / nLastModelMy);
         matWorld *= mat;
         
         /* Render object on the 3d stage */
         
         stage.setProjection (matProjection);
         stage.setCamera (matCamera);
         stage.enableAmbientLight (0.25f, 0.25f, 0.25f);
         stage.enableLight (0, K_LIGHT_DIR, 1, 1, 1, KVector3 (0, 0, 0), KVector3 (0, 0, -1), 1, 0, 0);
         stage.beginRender ();
         stage.renderModel (lpModel, 0, matWorld);
         stage.endRender ();
      }
      
      /* Draw caption, with a black outline */

      lpFont->setBlitColor (0, 0, 0, 0.125f * lpUIOptionsButton->getBlend());
      for (y = -1; y <= 1; y++) {
         for (x = -1; x <= 1; x++) {
            lpFont->drawStringFromLeftF (lpszCaption, 10 + x, 600 - 49 + y);
         }
      }
      lpFont->setBlitColor (1, 1, 1, lpUIOptionsButton->getBlend());
      lpFont->drawStringFromLeftF (lpszCaption, 10, 600 - 49);
      lpFont->setBlitColor (1, 1, 1, 1);

      if (lpUIOptionsWindow->getBlend() > 0) {
         /* Darken screen behind the options panel */
         lpBgGraphic->drawRect (-1, -1, 801, 601, 0, 0, 0, 0.333f * lpUIOptionsWindow->getBlend());
      }

      /* Draw UI */
      KUIElement::blitAll (0);

      /* Finish drawing this frame and present it */
      endFrame ();
   } while (!_lpKWindow->isQuit ());
   
   /* Schedule example local notification in 15 seconds with user data 54321 */
   bSuccess = KSysAndroid::scheduleLocalNotification (0, 0, 0, 15, false, "Test local notification", 54321);
   if (bSuccess)
      K_LOG ("Schedule local notification");
   else
      K_LOG ("Local notifications not supported");
   
   /* Clean up */

   delete lpButtonSnd;
   delete lpLaserSnd;
   
   KMiscTools::stopBackgroundMusic ();
   
   delete lpModel;
   
   delete lpBatch;

   delete lpUIContainer;   /* Deleting a UI element deletes all its children as well */
   KUIElement::cleanup ();

   delete lpFont;

   getGraphics()->unloadGraphicByRef (lpButtonGraphic);
   getGraphics()->unloadGraphicByRef (lpWindowGraphic);
   getGraphics()->unloadGraphicByRef (lpParticleGraphic);
   getGraphics()->unloadGraphicByRef (lpBgGraphic);
}

/**
 * Close game window and save settings. Derive and implement this method
 */

void MyGame::destroyGameWindow (KWindow *lpKWindow) {
   FILE *f;

   /* Close game window */
   delete lpKWindow;

   /* Store settings, if possible */
   KStringFmt (_szPathBuf, K_MAXPATH, "%s/settings.ini", getStateFolder());
   f = fopen (KMiscTools::makeFilePath (_szPathBuf), "w");
   if (f) {
      fprintf (f, "[settings]\n");
      fprintf (f, "music=%f\n", _fMusicVolume);
      fprintf (f, "sfx=%f\n", _fSfxVolume);
      fclose (f);
   }
}

/* Event handler */

bool MyGame::myEventHandler (KEvent *lpKEvent) {
   bool bProcessed, bSuccess;
   
   /* Send event to UI first */
   bProcessed = KUIElement::dispatchEvent (lpKEvent);
   if (!bProcessed) {
      /* Not caught by UI, handle */

      switch (lpKEvent->type) {
      case K_EVENT_ACTIVATED:
         /* Window got focus; resume audio */
         K_LOG ("Activated");
         KSound::resume ();
            
         /* Cancel local notifications */
         KSysAndroid::cancelLocalNotifications();
         break;

      case K_EVENT_DEACTIVATED:
         /* Window lost focus; suspend audio */
         K_LOG ("Deactivated");
         KSound::suspend ();
            
         /* Schedule example local notification in 15 seconds with user data 54321 */
         bSuccess = KSysAndroid::scheduleLocalNotification (0, 0, 0, 15, false, "Test local notification", 54321);
         if (bSuccess)
            K_LOG ("Schedule local notification");
         else
            K_LOG ("Local notifications not supported");
         break;
            
      case K_EVENT_LOCAL_NOTIFICATION:
         /* Local notification received */
         K_LOG ("Local notification: userdata=%ld", lpKEvent->notificationUserData);
         break;
         
      default:
         break;
      }
   }

   return true;
}

/* UI message handler */

void MyGame::myUIMessageHandler (KUIMessage *lpUIMessage) {
   if (lpUIMessage->nMessage == K_UIBUTTON_MSGFIRSTDOWN ||
       (lpUIMessage->nMessage == K_UISLIDER_MSGFIRSTDOWN && strcmp (lpUIMessage->lpElement->getName(), "music_slider"))) {
      /* Play a sound when any button or slider is clicked, except for the music volume slider */
      g_bPlayButtonSound = true;
   }
}

/** true if we need to play the 'button clicked' sound effect */
bool MyGame::g_bPlayButtonSound = false;

/** List of Particles */
KList<Particle> MyGame::g_lParticles;
