#!/bin/sh
rm -rf obj libs/armeabi* gen bin/classes bin/classes.dex bin/res bin/dexedLibs bin/R.txt bin/AndroidManifest.* bin/*
mkdir obj; mkdir obj/local; mkdir obj/local/armeabi-v7a obj/local/arm64-v8a obj/local/x86 obj/local/x86_64
cd jni
/cygdrive/c/Users/dell/AppData/Local/Android/Sdk/ndk/21.0.6113669/ndk-build.cmd
cd ..
ant release
cd bin
cp kgameexample-release-unsigned.apk kgameexample-release-tosign.apk
# keystore was created with:
# keytool -genkey -alias example -keyalg RSA -keysize 2048 -validity 10000 -keystore kgameexample.keystore -storepass examplepwd -keypass examplepwd
jarsigner -tsa http://timestamp.digicert.com -digestalg SHA1 -sigalg MD5withRSA -keystore ../kgameexample.keystore -storepass examplepwd -keypass examplepwd kgameexample-release-tosign.apk example
rm -f kgameexample.apk
/cygdrive/c/Users/dell/AppData/Local/Android/Sdk/build-tools/21.1.2/zipalign -v 4 kgameexample-release-tosign.apk kgameexample.apk
rm -f kgameexample-release-tosign.apk
cd ..



