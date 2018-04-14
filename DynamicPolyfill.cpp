#include<GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include<iterator>
#include<map>
#include<iostream>
#include<vector>
#define maxHt 1080
#define maxWd 1920
#define maxVer 10000

FILE *fp;

using namespace std;
struct Tuple
{
    int xEndPoints[2],yEndPoints[2];
    int yMax;
    int xOfyMin, xOfyMax;
    int dx,dy;
    int counter;
    int plotX;
    //float inverseSlope;
};

class SortedEdgeTable
{
    public:
        map<int,vector<Tuple>> buckets;
        int bucketsCount;
        void traverseEdgeTable();
        void sortTable();
        int deleteTuples(int bucketNo, int yMax);
        void incrementX(int bucketNo);
        void addTuple(int x1, int y1, int x2, int y2);
        void sortBucket(vector<Tuple> & bar);
        void mergeSortBucket(vector<Tuple>&left, vector<Tuple>& right, vector<Tuple>& bars);
};


void SortedEdgeTable::addTuple(int x1, int y1, int x2, int y2)
{
    if (y2==y1)
            return;
	int yMax,yMin, xOfyMin,xOfyMax, scanlineNo, dx, dy;
    dx=abs(x2-x1);
    dy=abs(y2-y1);
    scanlineNo=yMin=min(y1,y2);
    yMax=max(y1,y2);
	if (yMin==y1)
	{
	    xOfyMin=x1;
	    xOfyMax=x2;
	}
	else
    {
        xOfyMax=x1;
		xOfyMin=x2;
    }
    Tuple t;
    t.yMax=yMax;
    t.xOfyMin=xOfyMin;
    t.xOfyMax=xOfyMax;
    t.dx=dx;
    t.dy=dy;
    t.xEndPoints[0]=x1;t.xEndPoints[1]=x2;
    t.yEndPoints[0]=y1;t.yEndPoints[1]=y2;
    t.plotX=xOfyMin;
    t.counter=0;
    if(buckets.count(scanlineNo)==0)
    {
        vector<Tuple> vecT;
        vecT.push_back(t);
        buckets.insert(pair<int,vector<Tuple>>(scanlineNo,vecT));
    }
    else
        buckets.at(scanlineNo).push_back(t);
}

void SortedEdgeTable::incrementX(int bucketNo)
{
    vector<Tuple> * bucketVec = &buckets.at(bucketNo);
    for(vector<Tuple>::iterator it=(*bucketVec).begin(); it!=(*bucketVec).end(); ++it )
    {
        (*it).counter+=(*it).dx;
        if((*it).dx !=0 &&  (*it).counter >= (*it).dy )
        {
            (*it).counter=0;
            if((*it).xOfyMin < (*it).xOfyMax )
                (*it).plotX++;
            else
                (*it).plotX--;
        }
        //(*it).xOfyMin = (*it).xOfyMin + (*it).inverseSlope;
    }
}



int SortedEdgeTable::deleteTuples(int bucketNo, int yMax)
{
    int counter=0;
    vector<Tuple> * bucketVec = &buckets.at(bucketNo);
    vector<Tuple>::iterator it = (*bucketVec).begin();
    for(int i =0 ; i < (*bucketVec).size(); i++ )
        if((*bucketVec)[i].yMax == yMax ){
            (*bucketVec).erase(it+i);
            counter++;
            i--;
        }
    return counter;
}

void SortedEdgeTable::traverseEdgeTable()
{
    for( map<int,vector<Tuple>>::const_iterator mapIt = buckets.begin(); mapIt != buckets.end(); ++mapIt )
    {
        int key = (*mapIt).first;
        vector<Tuple> vect = (*mapIt).second;
        printf("Scanline(yMin) %d : \n",key);
        for(vector<Tuple>::iterator it=vect.begin(); it!=vect.end(); ++it )
            printf("(%d,%d) to (%d,%d) => (%d,%d,%d,%d,%d,%d) \n",(*it).xEndPoints[0],(*it).yEndPoints[0],(*it).xEndPoints[1],(*it).yEndPoints[1],(*it).dy,(*it).dx, (*it).xOfyMin, (*it).xOfyMax, (*it).counter, (*it).plotX);
        printf("\n");
    }
}

void SortedEdgeTable::sortBucket(vector<Tuple> & bar)
{
    if (bar.size() <= 1) return;
    int mid = bar.size() / 2;
    vector<Tuple> left;
    vector<Tuple> right;
    for (size_t j = 0; j < mid;j++)
        left.push_back(bar[j]);
    for (size_t j = 0; j < (bar.size()) - mid; j++)
        right.push_back(bar[mid + j]);
    sortBucket(left);
    sortBucket(right);
    mergeSortBucket(left, right, bar);
}

void structCopy(Tuple *t1, Tuple *t2)
{
    //Copy t2 to t1
    (*t1).yMax=(*t2).yMax;
    (*t1).xOfyMin=(*t2).xOfyMin;
    (*t1).xOfyMax=(*t2).xOfyMax;
    (*t1).dx=(*t2).dx;
    (*t1).dy=(*t2).dy;
    (*t1).plotX=(*t2).plotX;
    (*t1).xEndPoints[0]=(*t2).xEndPoints[0];
    (*t1).xEndPoints[1]=(*t2).xEndPoints[1];
    (*t1).yEndPoints[0]=(*t2).yEndPoints[0];
    (*t1).yEndPoints[1]=(*t2).yEndPoints[1];
}

void SortedEdgeTable::mergeSortBucket(vector<Tuple>&left, vector<Tuple>& right, vector<Tuple>& bars)
{
    int nL = left.size();
    int nR = right.size();
    int i = 0, j = 0, k = 0;

    while (j < nL && k < nR)
    {
        if (left[j].plotX < right[k].plotX) {
            structCopy(&bars[i],&left[j]);
            j++;
        }
        else {
            structCopy(&bars[i],&right[k]);
            k++;
        }
        i++;
    }
    while (j < nL) {
        structCopy(&bars[i],&left[j]);
        j++; i++;
    }
    while (k < nR) {
        structCopy(&bars[i],&right[k]);
        k++; i++;
    }
}

void SortedEdgeTable::sortTable()
{
    for( map<int,vector<Tuple>>::const_iterator mapIt = buckets.begin(); mapIt != buckets.end(); ++mapIt )
    {
        int key = (*mapIt).first;
        sortBucket(buckets.at(key));
    }
}

void scanlineFill(SortedEdgeTable * MainTable,int minY, int maxY,float r, float g, float b)
{
    int x1, yMax1, x2, yMax2, noOfIntersections =0 ;
    int fillPair = 0;
    SortedEdgeTable ActiveTable;
    (ActiveTable.buckets).insert(pair<int,vector<Tuple>>(0,vector<Tuple>()));
	for (int i=minY; i<maxY; i++)
	{
	    x1 = x2 = yMax1 = yMax2 = fillPair = noOfIntersections = 0;
	    int j=0;
	    if( ((*MainTable).buckets).count(i) != 0 )
        {
            vector<Tuple> vect = (*MainTable).buckets.at(i);
            for(vector<Tuple>::iterator it= vect.begin(); it!=vect.end(); ++it )
            {
                Tuple t1;
                t1.yMax=(*it).yMax;
                t1.xOfyMin=(*it).xOfyMin;
                t1.xEndPoints[0]=(*it).xEndPoints[0];
                t1.xEndPoints[1]=(*it).xEndPoints[1];
                t1.yEndPoints[0]=(*it).yEndPoints[0];
                t1.yEndPoints[1]=(*it).yEndPoints[1];
                t1.xOfyMax=(*it).xOfyMax;
                t1.dx=(*it).dx;
                t1.dy=(*it).dy;
                t1.plotX=(*it).plotX;
                t1.counter=(*it).counter;
                ActiveTable.buckets.at(0).push_back(t1);
            }
        }
        ActiveTable.deleteTuples(0,i);
        ActiveTable.sortTable();
        //printf("%d : ",i);
        //ActiveTable.traverseEdgeTable();
		while (j < ActiveTable.buckets[0].size() )
		{
			if ( noOfIntersections%2==0 )
				x1 = (ActiveTable.buckets[0])[j].plotX;
			else
			{
				x2 = (ActiveTable.buckets[0])[j].plotX;
				glColor3f(r,g,b);
				glBegin(GL_LINES);
				glVertex2i(x1+1,i);
				glVertex2i(x2-1,i);
				glEnd();
				glFlush();
			}
			noOfIntersections++;
			j++;
		}
        ActiveTable.incrementX(0);

	}
 }


void drawAndFill(SortedEdgeTable * MainTable, float r, float g, float b)
{
	glColor3f(1.0f,0.0f,0.0f);
	int count = 0,x1,y1,x2,y2;
	int maxY=0, minY=maxHt;
	rewind(fp);
	while(!feof(fp) )
	{
		count++;
		if (count>2)
		{
			x1 = x2;
			y1 = y2;
			count=2;
		}
		if (count==1){
			fscanf(fp, "%d,%d", &x1, &y1);
            if(y1<minY)minY=y1;
            if(y1>maxY)maxY=y1;
		}
		else
		{
			fscanf(fp, "%d,%d", &x2, &y2);
			if(y2<minY)minY=y2;
			if(y2>maxY)maxY=y2;
			glBegin(GL_LINES);
				glVertex2i( x1, y1);
				glVertex2i( x2, y2);
			glEnd();
			(*MainTable).addTuple(x1, y1, x2, y2);
		}
	}
	(*MainTable).sortTable();
	scanlineFill(MainTable,minY,maxY,r,g,b);
}

void polyFill()
{
    float r=0,g=0,b=1;
	SortedEdgeTable MainTable;
	drawAndFill(&MainTable,r,g,b);
}

void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,maxHt,0,maxWd);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char ** argv)
{
	fp=fopen ("D:\Graphics/dinoo.txt","r");
	if ( fp == NULL )
	{
		printf( "Could not open file" ) ;
		return 0;
	}
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(maxHt,maxWd);
	glutCreateWindow("Scanline Polyfill");
	myInit();
	glutDisplayFunc(polyFill);
	glutMainLoop();
	fclose(fp);
}

