#include "dxaudioclass.h"


void DirectXAudio::Init(void)
{
    // COM initialisieren
    CoInitialize(NULL);
    
    CoCreateInstance(CLSID_DirectMusicPerformance, NULL,
                     CLSCTX_INPROC, IID_IDirectMusicPerformance8,
                     (void**)&lpPerformance );
    
    CoCreateInstance(CLSID_DirectMusicLoader, NULL, 
                     CLSCTX_INPROC, IID_IDirectMusicLoader8,
                     (void**)&lpLoader);

	pGraph = NULL;
    pControl = NULL;
    pEvent = NULL;

    //CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
    //                    IID_IGraphBuilder, (void **)&pGraph);



    // Initialisieren
    lpPerformance->InitAudio( 
        NULL,                  
        NULL,                  
        NULL,                  
        DMUS_APATH_DYNAMIC_STEREO,
        8,                    
        DMUS_AUDIOF_ALL,       
        NULL                   
    );
    
    // Suchpfad auswählen
    WCHAR wstrSearchPath[MAX_PATH];
    MultiByteToWideChar( CP_ACP, 0, "res\\", -1, wstrSearchPath, MAX_PATH );
 
    // Suchpfad setzen
    lpLoader->SetSearchDirectory(GUID_DirectMusicAllTypes,  
        wstrSearchPath,            
        FALSE                      
    );
	
}


void DirectXAudio::AddBackgroundMusic(string Name)
{
    // Name der Datei
    WCHAR wstrFileName[MAX_PATH];
      
    MultiByteToWideChar(CP_ACP,0,Name.c_str(),-1,wstrFileName,MAX_PATH);
 
    if (FAILED(lpLoader->LoadObjectFromFile(
        CLSID_DirectMusicSegment,  
        IID_IDirectMusicSegment8,  
        wstrFileName,              
        (LPVOID*) &lpSegBackgroundMusic)))
    {
        MessageBox( NULL, "Fehler beim Laden der Datei", 
                          "AddBackgroundMusic", MB_OK );   
    }

    // Segment hinzufügen
    lpSegBackgroundMusic->Download( lpPerformance );
}


void DirectXAudio::AddSound(string Name)
{
    WCHAR wstrFileName[MAX_PATH]; 
    
    MultiByteToWideChar(CP_ACP,0,Name.c_str(),-1,wstrFileName,MAX_PATH);

    if (FAILED(lpLoader->LoadObjectFromFile(
        CLSID_DirectMusicSegment,  
        IID_IDirectMusicSegment8,  
        wstrFileName,              
        (LPVOID*) &SoundEffectsMap[Name])))
    {
        MessageBox( NULL, "Fehler beim Laden der Datei","AddSound", MB_OK );
        return;
    }

    // Segment hinzufügen
    SoundEffectsMap[Name]->Download(lpPerformance);  
}


void DirectXAudio::PlayBackgroundMusic(void)
{
  lpPerformance->PlaySegmentEx(
        lpSegBackgroundMusic,           // abzuspielendes Segment
        NULL,        
        NULL,        
        DMUS_SEGF_SECONDARY,  // im sekundären Buffer
        0,                    // Stelle, an der gestartet werden soll
        NULL,        
        NULL,        
        NULL         
    );   
}

void DirectXAudio::PlaySoundEffect(string Name)
{
    lpPerformance->PlaySegmentEx(
        SoundEffectsMap[Name],           // abzuspielendes Segment
        NULL,        
        NULL,        
        DMUS_SEGF_SECONDARY,  // im sekundären Buffer
        0,                    // Stelle, an der gestartet werden soll
        NULL,        
        NULL,        
        NULL         
    );   
}

DirectXAudio::~DirectXAudio()
{
    map<string,IDirectMusicSegment8*>::iterator it;
    for(it=SoundEffectsMap.begin();it!=SoundEffectsMap.end();it++)    
    {
        (*it).second->Release();
    }
    

    // Segment freigeben
    if(lpSegBackgroundMusic)
    {
        lpSegBackgroundMusic->Release();
        lpSegBackgroundMusic = NULL;
    }
  
    // performance freigeben
    if(lpPerformance)
    {
        lpPerformance->Stop(NULL,NULL,0,0);
        lpPerformance->CloseDown();
        lpPerformance->Release();
        lpPerformance = NULL;
    }

    // Loader freigeben
    if(lpLoader)
    {
        lpLoader->Release(); 
        lpLoader = NULL;
    }
        
    // COM beenden
    CoUninitialize();
}

void DirectXAudio::PlayMP3File(void)
{
    
	pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

    
    pGraph->RenderFile(L"res\\music.mp3", NULL);
	pControl->Run();
        
//	long evCode;
    //pEvent->WaitForCompletion(INFINITE, &evCode);
//	pEvent->WaitForCompletion(5000,&evCode);

}
void DirectXAudio::StopMP3File(void)
{
    
	pControl->Stop();
	pControl->Release();
    pEvent->Release();
    pGraph->Release();
	//pControl->Release();
    //pEvent->Release();
    //pGraph->Release();
    
}