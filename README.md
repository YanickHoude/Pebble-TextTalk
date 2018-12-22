# TextTalk

A way of making communication easier for those with all ranges of hearing impairment by trascribing live conversation to text and 
displaying it on the Pebble watch interface. 

## Built With

* [Pebble-SDK](https://developer.rebble.io/developer.pebble.com/docs/index.html) - Devkit
* [Android.Speech](https://developer.android.com/reference/android/speech/package-summary) - Transcription engine

## How it Works

There are two parts to TextTalk. The first is the Pebble watch application, which is comprised of a simple interface that displays active conversation. The Pebble interface was written in C using CloudPebble, the online IDE provided by Pebble for its developers. The second part is the Android companion app. This application uses the Android Speech to Text API to translate speech into text when prompted by the user through the Pebble TextTalk app. The companion app was developed in Android Studio and was written in Java.

## Authors

* **Jake Raw** - *Front End UI* - [jraw96](https://github.com/jraw96)
* **Henry Tran** - *Android App* - [hnrytrn](https://github.com/hnrytrn)
* **Siran Thirugnanam** - *Android App* - [serant](https://github.com/serant)
* **Yanick Hode** - *Pebble App* - [YanickHoude](https://github.com/YanickHoude)


