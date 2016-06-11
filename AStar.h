#ifndef ASTAR_H
#define ASTAR_H

class Map;

typedef struct _APoint{
	int x;
	int y;
	int type;
	int f;
	int g;
	int h;
} APoint,*PAPoint;

enum APointType{
	APT_UNKNOWN,
	APT_OPENED,
	APT_CLOSED,
	APT_STARTPOINT,
	APT_ENDPOINT
};


class AStarBase{
public:
	AStarBase();
	~AStarBase();

public:
	bool Create(Map* pDateArr);
	void SetStartPoint(int x,int y);
	void SetEndPoint(int x,int y);
	void SetOpened(int x,int y);
	void SetClosed(int x,int y);
	void SetCurrent( int x,int y );

	PAPoint CalcNextPoint(PAPoint ptCalc);

private:
	PAPoint m_pAPointArr;
	int m_nAPointArrWidth;
	int m_nAPointArrHeight;

	PAPoint m_pStartPoint,m_pEndPoint,m_pCurPoint;
	Map* m_pOldArr;
};


#endif // ASTAR_H
