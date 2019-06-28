#ifndef _DirectXAudio_
#define _DirectXAudio_


#include <dmusici.h>

#pragma warning(disable: 4786)

#include <string>
#include <map>

using namespace std;

// Definition der Klasse DirectXAudio

class DirectXAudio
{
     public:

        ~DirectXAudio();

        // DirectXAudio initialisieren
        void Init(void);
        
        // Musik und Soundeffekte hinzufügen
        void AddBackgroundMusic(string Name);
        void AddSound(string Name);

        // Musik und Soundeffekte abspielen
        void PlayBackgroundMusic(void);
        void PlaySoundEffect(string Name);

    private:

        // DirectXMusic Objekte
        IDirectMusicPerformance8* lpPerformance;
        IDirectMusicLoader8*      lpLoader;
        IDirectMusicSegment8*     lpSegBackgroundMusic;
        
        
        map<string,IDirectMusicSegment8*> SoundEffectsMap;

        // die Hintergrundmusik
        string bgMusic;
};

#endif