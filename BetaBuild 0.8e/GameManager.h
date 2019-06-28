#include "sprite.h"
#include "RessourceManager.h"
#include "DXAudioClass.h"

#include <list>

using namespace std;


// die Klasse GameManager
class CGameManager
{
    public:

        CGameManager();
        virtual ~CGameManager();
    
        void Init(LPDIRECT3DDEVICE9 lpDevice,CResourceManager* lpResManager,DirectXAudio* lpDXAudio, CSprite* lpSnake);

        int Update();
		void CreateBody(D3DXVECTOR3 Position,float Rotation);
		void CreateBodyB(D3DXVECTOR3 Position,float Rotation);
        void Draw(void);

        void Reset(void);

        BOOL IsPlayerDestroyed(void);
        
        unsigned int  SetDestroyed(void);
        int  GetLives(void);
        int  GetLevel(void);
        int  GetPoints(void);
        int  GetHighscore(void);
		unsigned int m_Points;
    protected:

        // Objekte erzeugen
        void CreateAsteroid(void);

        unsigned int m_Lives;
        //unsigned int m_Points;
        unsigned int m_Highscore;

        BOOL m_PlayerDestroyed;        

        // Name der Texturen - zur Zwischenspeicherung
        char m_TextureName[256];

        LPDIRECT3DDEVICE9 m_lpD3DDevice;
		CSprite*		  m_lpSnake;
        DirectXAudio*     m_lpDXAudio;
        CResourceManager* m_lpResManager;
		list<CSprite*> m_BodyList;
        list<CSprite*>::iterator it;
};