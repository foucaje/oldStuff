#include "GameManager.h"

// Konstruktor - Initialisierungen
CGameManager::CGameManager()
{
    m_Lives = 3;

    m_Points    = 0;
    m_Highscore = 0;

    m_PlayerDestroyed = FALSE;

    m_lpDXAudio    = NULL;
    m_lpD3DDevice  = NULL;
    m_lpResManager = NULL;
}

// Destruktor - Objekte freigeben
CGameManager::~CGameManager()
{

	for(it=m_BodyList.begin();it!=m_BodyList.end();it++)
    {
        delete (*it);
    }
    
    m_BodyList.clear();
	
	Reset();
}

// Speicher, der für die Sprites reserviert wurde wieder frei geben
void CGameManager::Reset(void)
{
    // Variablen zurücksetzen
    m_Lives  = 1;
    m_Points = 0;

	for(it=m_BodyList.begin();it!=m_BodyList.end();it++)
    {
        delete (*it);
    }
    
    m_BodyList.clear();
	
}

void CGameManager::Init(LPDIRECT3DDEVICE9 lpDevice,CResourceManager* lpResManager,DirectXAudio* lpDXAudio, CSprite* lpSnake)
{
    m_lpD3DDevice  = lpDevice;
    m_lpResManager = lpResManager;
    m_lpDXAudio    = lpDXAudio;
	m_lpSnake      = lpSnake;
}  


BOOL CGameManager::IsPlayerDestroyed(void)
{
    return m_PlayerDestroyed;
}

unsigned int CGameManager::SetDestroyed()
{
    return --m_Lives;
}

int CGameManager::GetLives(void)
{
    return m_Lives;
}


int CGameManager::GetPoints(void)
{
    return m_Points;
}

int CGameManager::GetHighscore(void)
{
    return m_Highscore;
}


int CGameManager::Update()
{
    // Framerate holen
    int Framerate = m_lpResManager->GetFramerate();

	int oldX, oldY;

	for(it=m_BodyList.begin();it!=m_BodyList.end();it++)
    {
		
		if (it==m_BodyList.begin())
		{
			float a,b,a2,b2;
			m_lpSnake->GetPosition(a,b);		// Position des Kopfes
			(*it)->GetPosition(a2,b2);			// Position des Rumpfes
			
			oldX=a2;
			oldY=b2;

			(*it)->SetPosition(a,b);
			/*float a_1, a_2, b_1, b_2;			// alte methode (mit Loch)
				a_1=a2-a;
				a_2=a-a2;
				b_1=b2-b;
				b_2=b-b2;

				
					if (a_1>10)
					{
						(*it)->SetPosition(a+10,b);
					}
					if (a_2>10)
					{
						(*it)->SetPosition(a-10,b);
					}
					if (b_1>10)
					{
						(*it)->SetPosition(a,b+10);
					}
					if (b_2>10)
					{
						(*it)->SetPosition(a,b-10);
					}*/
				
			
			
		} else {

			float x,y;
			int tmpX=oldX, tmpY=oldY;
			(*it)->GetPosition(x,y);
						if ((*it)->GetPosition() == m_lpSnake->GetPosition())
						return 0;
			oldX=x;
			oldY=y;
			(*it)->SetPosition(tmpX,tmpY);
		}
		
    }
    
    return 1;
}



// Sprites zeichnen
void CGameManager::Draw(void)
{
    // durch alle Listen laufen und jedes Objekt zeichnen
	    for(it=m_BodyList.begin();it!=m_BodyList.end();it++)
    {
        (*it)->Draw();
    }

}

// Sprite für eine Photonenschuss erzeugen
void CGameManager::CreateBody(D3DXVECTOR3 Position,float Rotation)
{
    CSprite* Body = new CSprite;

    
	sprintf(m_TextureName,"res\\SnakeBody.png");
	Body->Create(m_lpD3DDevice,m_TextureName);   

    // die Texturen für den Schuss zuweisen
   	sprintf(m_TextureName,"res\\SnakeBody.png");
    Body->AddTexture(m_lpResManager->GetTexture(m_TextureName));      


    // Rotation in Radianten umrechnen
    float TempRot = Rotation * D3DX_PI / 180.0f;

    float x,y;

	m_lpSnake->GetPosition(x,y);
	Body->SetPosition(0,0);
    Body->SetRotation(Rotation);
	m_BodyList.push_back(Body);
}

void CGameManager::CreateBodyB(D3DXVECTOR3 Position,float Rotation)
{
    CSprite* Body = new CSprite;

    
	sprintf(m_TextureName,"res\\SnakeBodyB.png");
	Body->Create(m_lpD3DDevice,m_TextureName);   

    // die Texturen für den Schuss zuweisen
   	sprintf(m_TextureName,"res\\SnakeBodyB.png");
    Body->AddTexture(m_lpResManager->GetTexture(m_TextureName));      


    // Rotation in Radianten umrechnen
    float TempRot = Rotation * D3DX_PI / 180.0f;

    float x,y;

	m_lpSnake->GetPosition(x,y);
	Body->SetPosition(0,0);
    Body->SetRotation(Rotation);
	m_BodyList.push_back(Body);
}
