#include "sprite.h"
#include "fsUtility.h"

CFSLogger cLog;
char buffer[128];

// Konstruktor - Initialisierungen
CSprite::CSprite(void)
{
    m_lpSprite  = NULL;
    m_lpTexture = NULL;
	m_Look = 4;
}

// Destructor - freigeben der Objekte
CSprite::~CSprite(void)
{
    if(NULL != m_lpSprite)
    {
        m_lpSprite->Release();
        m_lpSprite = NULL;
    }

    if(NULL != m_lpTexture)
    {
        m_lpTexture->Release();
        m_lpTexture = NULL;
    }
}

// Position setzen
void CSprite::SetPosition(float x,float y)
{
    m_vPosition.x = x;
    m_vPosition.y = y;
}

// Rotation setzen
void CSprite::SetRotation(float rotation)
{
    m_Rotation = rotation;
}

// Skalierungsfaktor setzen
void CSprite::SetScale(float scale)
{
    m_vScale.x = scale;
    m_vScale.y = scale;
}

// Schrittweite bestimmen
void CSprite::SetStep(float StepX,float StepY)
{
    m_StepX = StepX;
    m_StepY = StepY;
}

// Sprite kreiern
void CSprite::Create(LPDIRECT3DDEVICE9 lpDevice, LPCSTR lpFileName)
{
    int nReductionFactor = 1;
	m_lpDevice  = lpDevice;
	
    // Sprite anlegen
    if (FAILED(D3DXCreateSprite(m_lpDevice,&m_lpSprite)))
	{	
		sprintf(buffer,"Could not Create Sprite");
		cLog.out(buffer);
	}

    // Inforationen über die Textur holen
    D3DXIMAGE_INFO ImgInfo;
    D3DXGetImageInfoFromFile(lpFileName,&ImgInfo);

    // Textur anlegen
    if (FAILED(D3DXCreateTextureFromFileEx(m_lpDevice,
                                lpFileName,
                                ImgInfo.Width,
                                ImgInfo.Height,
                                1,0,
                                D3DFMT_UNKNOWN,
                                D3DPOOL_MANAGED,
                                D3DX_FILTER_NONE,
                                D3DX_FILTER_NONE,
                                0,0,0,
                                &m_lpTexture)))
	{
	sprintf(buffer,"Could not load Texture");
	cLog.out(buffer);	
	}

    // Breite und Höhe speichern
    m_Width  = ImgInfo.Width;
    m_Height = ImgInfo.Height;

//	sprintf(buffer,"Berechnete breite und höhe -> %d %d", m_Width, m_Height);
//	cLog.out(buffer);
	
	// Reduzierte Version des Sprites erzeugen
	m_lpReducedImage = new BYTE[m_Width / nReductionFactor * m_Height / nReductionFactor];
	
	// Textur sperren
	D3DLOCKED_RECT LockedRect;

	m_lpTexture->LockRect(0,&LockedRect,NULL,D3DLOCK_READONLY);

	DWORD* pPixels = (DWORD*)LockedRect.pBits;

	LockedRect.Pitch /= 4;

	DWORD i,j;
	
	// reduzierten Puffer füllen
	for(i=0;i<m_Height;i+=nReductionFactor)
	{		
		for(j=0;j<m_Width;j+=nReductionFactor)
		{
			if(pPixels[i*LockedRect.Pitch + j] != 0)
			{
				m_lpReducedImage[i/nReductionFactor * m_Width/nReductionFactor + j/nReductionFactor] = 1;
			}
			else
			{
				m_lpReducedImage[i/nReductionFactor * m_Width/nReductionFactor + j/nReductionFactor] = 0;
			}
		}				
	}

    m_vCenter = D3DXVECTOR3((float)m_Width/2,(float)m_Height/2,1.0f);

	m_vPosition = D3DXVECTOR3((float)m_Width/2,(float)m_Height/2,1.0f);

    // Skalierung und Schrittweite setzen
    m_vScale = D3DXVECTOR3(1.0f,1.0f,1.0f);

    // Schrittweite des Sprites bei Bewegung
    m_StepX  = 10.0f;
    m_StepY  = 10.0f;
}

// Bewegung des Sprite
short CSprite::Move(int dir, bool typ)
{
    short oldLook = m_Look;
	// Bei der Bewegung werden die Bildschirmbegrenzungen beachtet
	if (typ == false)
	{
		switch(dir)
		{
		case 1:	
			{	
					if (m_Direction != 2)
					{
						m_Direction=1;
						m_Look=1;
					}
					break;
			}

        case 2:
			{		
					if (m_Direction != 1)
					{
						m_Direction=2;
						m_Look=2;
					}
					break;
			}
        case 4:
			{		
					if (m_Direction != 8)
					{
						m_Direction=4;
						m_Look=4;
					}
					break;
			}
        case 8:
			{		
					if (m_Direction != 4)
					{
						m_Direction=8;
						m_Look=8;
					}
					break;
			}
		}
	} else {
		
		switch(dir)
		{
		case 64:	
			{	
					if (m_Direction != 2)
					{
						m_Direction=1;
						m_Look=1;
					}
					break;
			}

        case 65:
			{		
					if (m_Direction != 1)
					{
						m_Direction=2;
						m_Look=2;
					}
					break;
			}
		case 66:
			{		
					if (m_Direction != 8)
					{
						m_Direction=4;
						m_Look=4;
					}
					break;
			}
        case 67:
			{		
					if (m_Direction != 4)
					{
						m_Direction=8;
						m_Look=8;
					}
					break;
			}
		}	
	
	}

	switch(m_Look)
	{
	case 1:
		{
			/*if (oldLook == 2)
			{
				Rotate((float)180*D3DX_PI/180);
			}*/
			if (oldLook == 4)
			{
				Rotate((float)-90*D3DX_PI/180);
			}
			if (oldLook == 8)
			{
				Rotate((float)90*D3DX_PI/180);
			}

		}
		break;
	
	case 2:
		{
			/*if (oldLook == 1)
			{
				Rotate((float)180*D3DX_PI/180);
			}*/
			if (oldLook == 4)
			{
				Rotate((float)90*D3DX_PI/180);
			}
			if (oldLook == 8)
			{
				Rotate((float)-90*D3DX_PI/180);
			}

		}
		break;
	case 4:
		{
			if (oldLook == 1)
			{
				Rotate((float)90*D3DX_PI/180);
			}
			if (oldLook == 2)
			{
				Rotate((float)-90*D3DX_PI/180);
			}
			/*if (oldLook == 8)
			{
				Rotate((float)180*D3DX_PI/180);
			}*/

		}
		break;
	case 8:
		{
			/*if (oldLook == 4)
			{
				Rotate((float)180*D3DX_PI/180);
			}*/
			if (oldLook == 2)
			{
				Rotate((float)90*D3DX_PI/180);
			}
			if (oldLook == 1)
			{
				Rotate((float)-90*D3DX_PI/180);
			}

		}
		break;
			
	
	}
	
	switch(m_Direction)
	{
	case 1:	{
			m_vPosition.x-=m_StepX;
				if (m_vPosition.x<112)
				return 0;
			}
			break;
	
	case 2: {
			m_vPosition.x+=m_StepX;
				if (m_vPosition.x>912)
					return 0;
			}
		   break;

	case 4:	{
			m_vPosition.y-=m_StepY;
			if (m_vPosition.y<30)
				return 0;
			}
			break;

	case 8:	{
			m_vPosition.y+=m_StepY;
			if (m_vPosition.y>630)
				return 0;
			}
			break;
	}
return 1;
}

// Skalierung des Sprites
void CSprite::Scale(float dScale)
{
    m_vScale.x += dScale;
    m_vScale.y += dScale;
}

// Rotation des Sprites
void CSprite::Rotate(float dRotation)
{
    m_Rotation += dRotation;
}

// Sprite zeichnen
void CSprite::Draw(void)
{
         // Zeichnen des Sprites

		// Transformationsmatrizen angeben - neu im Summer Update 2003
		D3DXMATRIXA16 matCombo, matRot, matScale, matTrans;

		D3DXMatrixRotationZ(&matRot,m_Rotation);
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		
		D3DXMatrixMultiply(&matCombo, &matRot, &matScale);
		D3DXMatrixMultiply(&matCombo, &matCombo, &matTrans);

		m_lpSprite->Begin(D3DXSPRITE_DONOTSAVESTATE | D3DXSPRITE_ALPHABLEND);

		m_lpSprite->SetTransform(&matCombo);

         // Sprite in der passenden Animationsstufe zeichnen
        m_lpSprite->Draw(m_lpTexture,
					     NULL,
                         &m_vCenter,
                         NULL,
                         0xFFFFFFFF);

		m_lpSprite->End();
		
}

// Reduziertes Sprite (Image) zurückgeben
BYTE* CSprite::GetReducedImage(void)
{ 
	return m_lpReducedImage; 
}

// Position des Sprites zurückgeben
D3DXVECTOR3 CSprite::GetPosition(void) 
{ 
	return m_vPosition; 
}

void CSprite::GetPosition(float &x, float &y)
{
	x = m_vPosition.x;
	y = m_vPosition.y;
}

// Textur für das Sprite festlegen
void CSprite::AddTexture(LPDIRECT3DTEXTURE9 lpTexture)
{
    // Beschreibung des zu Grunde liegenden Surfaces
    D3DSURFACE_DESC desc;

    lpTexture->GetLevelDesc(0,&desc);
    
    // Breite und Höhe speichern
    m_Width  = desc.Width;
    m_Height = desc.Height;
    
    // Zeiger auf die Textur speichern
//    m_AnimationVec.push_back(lpTexture);
}