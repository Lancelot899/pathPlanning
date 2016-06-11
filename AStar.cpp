
#include "AStar.h"
#include "Mapper.h"

AStarBase::AStarBase()
{
	m_pAPointArr = nullptr;
	m_nAPointArrWidth = 0;
	m_nAPointArrHeight = 0;

	m_pStartPoint = nullptr;
	m_pEndPoint = nullptr;
	m_pCurPoint = nullptr;
}

AStarBase::~AStarBase() {}

bool AStarBase::Create(Map *pDateArr)
{
	if(!pDateArr)
		return false;

	m_pAPointArr = new APoint[pDateArr->getWidth()*pDateArr->getHeight()];

	if(!m_pAPointArr)
		return false;

	m_pOldArr = pDateArr;
	m_nAPointArrWidth = pDateArr->getWidth();
	m_nAPointArrHeight = pDateArr->getHeight();

	for ( int y = 0;y<m_nAPointArrHeight;y++)
	{
		for ( int x=0;x<m_nAPointArrWidth;x++)
		{
			m_pAPointArr[y*m_nAPointArrWidth+x].x = x;
			m_pAPointArr[y*m_nAPointArrWidth+x].y = y;
			m_pAPointArr[y*m_nAPointArrWidth+x].g = 0;
			m_pAPointArr[y*m_nAPointArrWidth+x].f = 0;
			m_pAPointArr[y*m_nAPointArrWidth+x].h = 0;

			if ( pDateArr->at(x,y)== 0)
				m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_OPENED;

			else if ( pDateArr->at(x,y) == MapType::Obstacle)
			{
				m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_CLOSED;
				if(x - 1 >= 0)
					m_pAPointArr[y*m_nAPointArrWidth+x - 1].type = APT_CLOSED;
				if(y - 1 >= 0)
					m_pAPointArr[(y - 1)*m_nAPointArrWidth+x].type = APT_CLOSED;
				if(x - 1 >= 0 && y - 1 >= 0)
					m_pAPointArr[(y - 1)*m_nAPointArrWidth+x - 1].type = APT_CLOSED;
				if(x - 1 >= 0 && y + 1 < m_nAPointArrHeight)
					m_pAPointArr[(y + 1)*m_nAPointArrWidth+x - 1].type = APT_CLOSED;
				if(x + 1 < m_nAPointArrWidth)
					m_pAPointArr[y*m_nAPointArrWidth+x + 1].type = APT_CLOSED;
				if(x + 1 < m_nAPointArrWidth && y - 1 >= 0)
					m_pAPointArr[(y - 1)*m_nAPointArrWidth+x + 1].type = APT_CLOSED;
				if(x + 1 < m_nAPointArrWidth && y + 1 < m_nAPointArrHeight)
					m_pAPointArr[(y + 1)*m_nAPointArrWidth+x + 1].type = APT_CLOSED;
				if(y + 1 < m_nAPointArrHeight)
					m_pAPointArr[(y + 1)*m_nAPointArrWidth+x].type = APT_CLOSED;
			}

			else if ( pDateArr->at(x,y) == MapType::Begin)
			{
				m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_STARTPOINT;
				m_pStartPoint = m_pAPointArr + y*m_nAPointArrWidth+x;
				m_pCurPoint = m_pStartPoint;
			}

			else if ( pDateArr->at(x,y) == MapType::End)
			{
				m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_ENDPOINT;
				m_pEndPoint = m_pAPointArr + y*m_nAPointArrWidth+x;
			}

			else
				m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_UNKNOWN;

		}
	}
	return true;
}

void AStarBase::SetStartPoint( int x,int y )
{
	if ( m_pStartPoint && m_pAPointArr[y*m_nAPointArrWidth+x].type!=APT_CLOSED )
	{
		m_pStartPoint->type = APT_OPENED;
		m_pStartPoint = m_pAPointArr + y*m_nAPointArrWidth+x;
		m_pStartPoint->type = APT_STARTPOINT;
		m_pCurPoint = m_pStartPoint;
	}
}

void AStarBase::SetEndPoint( int x,int y )
{
	if ( m_pStartPoint && m_pAPointArr[y*m_nAPointArrWidth+x].type!=APT_CLOSED )
	{
		m_pStartPoint->type = APT_OPENED;
		m_pStartPoint = m_pAPointArr + y*m_nAPointArrWidth+x;
		m_pStartPoint->type = APT_ENDPOINT;
	}
}

void AStarBase::SetCurrent( int x,int y )
{
	m_pCurPoint = m_pAPointArr+y*m_nAPointArrWidth+x;
}

void AStarBase::SetOpened( int x,int y )
{
	if ( m_pAPointArr[y*m_nAPointArrWidth+x].type!=APT_OPENED )
	{
		m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_OPENED;
	}
}

void AStarBase::SetClosed( int x,int y )
{
	if ( m_pAPointArr[y*m_nAPointArrWidth+x].type!=APT_CLOSED )
	{
		m_pAPointArr[y*m_nAPointArrWidth+x].type = APT_CLOSED;
	}
}

PAPoint AStarBase::CalcNextPoint( PAPoint ptCalc )
{
	if ( ptCalc == nullptr )
		ptCalc = m_pStartPoint;

	int x = ptCalc->x;
	int y = ptCalc->y;
	int dx = m_pEndPoint->x;
	int dy = m_pEndPoint->y;
	int xmin = x,ymin = y,vmin = 0;

	if ( (x==dx && abs(y-dy)==1) || (y==dy && abs(x-dx)==1) )
		return m_pEndPoint;

	// up
	if ( m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].type == APT_OPENED && y>0)
	{
		m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].g = 10;
		m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].h =
			10*(abs(x - dx) + abs(y-1 - dy));
		m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].f =
			m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].g + m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].h;
		if ( vmin==0 )
		{
			xmin = x;
			ymin = y-1;
			vmin = m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].f;
		}else{
			if ( vmin > m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].f )
			{
				xmin = x;
				ymin = y-1;
				vmin = m_pAPointArr[(x+0)+m_nAPointArrWidth*(y-1)].f;
			}
		}
	}

	// down
	if ( m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].type == APT_OPENED && y<m_nAPointArrHeight)
	{
		m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].g = 10;
		m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].h =
			10*(abs(x - dx) + abs(y+1 - dy));
		m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].f =
			m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].g + m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].h;
		if ( vmin==0 )
		{
			xmin = x;
			ymin = y+1;
			vmin = m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].f;
		}else{
			if ( vmin > m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].f )
			{
				xmin = x;
				ymin = y+1;
				vmin = m_pAPointArr[(x+0)+m_nAPointArrWidth*(y+1)].f;
			}
		}
	}

	// left
	if ( m_pAPointArr[(x-1)+m_nAPointArrWidth*y].type == APT_OPENED && x>0)
	{
		m_pAPointArr[(x-1)+m_nAPointArrWidth*y].g = 10;
		m_pAPointArr[(x-1)+m_nAPointArrWidth*y].h =
			10*(abs(x-1 - dx) + abs(y - dy));
		m_pAPointArr[(x-1)+m_nAPointArrWidth*y].f =
			m_pAPointArr[(x-1)+m_nAPointArrWidth*y].g + m_pAPointArr[(x-1)+m_nAPointArrWidth*y].h;
		if ( vmin==0 )
		{
			xmin = x-1;
			ymin = y;
			vmin = m_pAPointArr[(x-1)+m_nAPointArrWidth*y].f;
		}else{
			if ( vmin > m_pAPointArr[(x-1)+m_nAPointArrWidth*y].f )
			{
				xmin = x-1;
				ymin = y;
				vmin = m_pAPointArr[(x-1)+m_nAPointArrWidth*y].f;
			}
		}
	}

	// right
	if ( m_pAPointArr[(x+1)+m_nAPointArrWidth*y].type == APT_OPENED && x<m_nAPointArrWidth)
	{
		m_pAPointArr[(x+1)+m_nAPointArrWidth*y].g = 10;
		m_pAPointArr[(x+1)+m_nAPointArrWidth*y].h =
			10*(abs(x+1 - dx) + abs(y - dy));
		m_pAPointArr[(x+1)+m_nAPointArrWidth*y].f =
			m_pAPointArr[(x+1)+m_nAPointArrWidth*y].g + m_pAPointArr[(x+1)+m_nAPointArrWidth*y].h;

		if ( vmin==0 )
		{
			xmin = x+1;
			ymin = y;
			vmin = m_pAPointArr[(x+1)+m_nAPointArrWidth*y].f;
		}
		else{
			if ( vmin > m_pAPointArr[(x+1)+m_nAPointArrWidth*y].f )
			{
				xmin = x+1;
				ymin = y;
				vmin = m_pAPointArr[(x+1)+m_nAPointArrWidth*y].f;
			}
		}
	}

	//no opt point return nullptr
	if ( vmin )
	{
		SetCurrent(xmin,ymin);
		SetClosed(xmin,ymin);
		PAPoint pApoint = CalcNextPoint(m_pCurPoint);

		if ( pApoint == nullptr )
		{
			SetCurrent(x,y);
			SetClosed(xmin,ymin);
			return CalcNextPoint(m_pCurPoint);
		}
		return pApoint;
	}

	else
		return nullptr;
}

