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
    float xOfyMin;
    float inverseSlope;
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
    float slope,inverseSlope, xOfyMin;
	int yMax, yMin, scanlineNo;
    if (y2==y1)
            return;
	if (x1==x2)
		inverseSlope=0.0;
	else
    {
        slope = ((float)(y2-y1))/((float)(x2-x1));
        inverseSlope=1.0/slope;
    }
    scanlineNo=yMin=min(y1,y2);
    yMax=max(y1,y2);
	if (yMin==y1)
		xOfyMin=x1;
	else
		xOfyMin=x2;
    Tuple t;
    t.yMax=yMax;
    t.xOfyMin=xOfyMin;
    t.inverseSlope=inverseSlope;
    t.xEndPoints[0]=x1;t.xEndPoints[1]=x2;
    t.yEndPoints[0]=y1;t.yEndPoints[1]=y2;
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
        (*it).xOfyMin = (*it).xOfyMin + (*it).inverseSlope;
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
            printf("(%d,%d) to (%d,%d) (%d,%f,%f) \n",(*it).xEndPoints[0],(*it).yEndPoints[0],(*it).xEndPoints[1],(*it).yEndPoints[1],(*it).yMax,(*it).xOfyMin,(*it).inverseSlope);
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
    (*t1).xEndPoints[0]=(*t2).xEndPoints[0];
    (*t1).xEndPoints[1]=(*t2).xEndPoints[1];
    (*t1).yEndPoints[0]=(*t2).yEndPoints[0];
    (*t1).yEndPoints[1]=(*t2).yEndPoints[1];
    (*t1).inverseSlope=(*t2).inverseSlope;
}

void SortedEdgeTable::mergeSortBucket(vector<Tuple>&left, vector<Tuple>& right, vector<Tuple>& bars)
{
    int nL = left.size();
    int nR = right.size();
    int i = 0, j = 0, k = 0;

    while (j < nL && k < nR)
    {
        if (left[j].xOfyMin < right[k].xOfyMin) {
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

void scanlineFill(SortedEdgeTable * MainTable)
{
    int x1, yMax1, x2, yMax2, noOfIntersections =0 ;
    int fillPair = 0;
    SortedEdgeTable ActiveTable;
    (ActiveTable.buckets).insert(pair<int,vector<Tuple>>(0,vector<Tuple>()));
	for (int i=0; i<maxHt; i++)
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
                t1.inverseSlope=(*it).inverseSlope;
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
			{
				x1 =(int) (ActiveTable.buckets[0])[j].xOfyMin;
				yMax1 = (ActiveTable.buckets[0])[j].yMax;
                noOfIntersections++;
			}
			else
			{
				x2 = (int)(ActiveTable.buckets[0])[j].xOfyMin;
				yMax2 = (ActiveTable.buckets[0])[j].yMax;
                noOfIntersections++;
				glColor3f(0.0f,0.7f,0.0f);
				glBegin(GL_LINES);
				glVertex2i(x1+1,i);
				glVertex2i(x2-1,i);
				glEnd();
				glFlush();
			}
			j++;
		}
        ActiveTable.incrementX(0);

	}
 }


void drawOutline(SortedEdgeTable * MainTable)
{
	glColor3f(1.0f,0.0f,0.0f);
	int count = 0,x1,y1,x2,y2;
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
		if (count==1)
			fscanf(fp, "%d,%d", &x1, &y1);
		else
		{
			fscanf(fp, "%d,%d", &x2, &y2);
			glBegin(GL_LINES);
				glVertex2i( x1, y1);
				glVertex2i( x2, y2);
			glEnd();
			(*MainTable).addTuple(x1, y1, x2, y2);
		}
	}
	(*MainTable).sortTable();
}

void polyFill()
{
	SortedEdgeTable MainTable;
	drawOutline(&MainTable);
	scanlineFill(&MainTable);
	//MainTable.traverseEdgeTable();
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

