#include<windows.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include "transformation.h"
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
        //printf("Scanline(yMin) %d : \n",key);
        for(vector<Tuple>::iterator it=vect.begin(); it!=vect.end(); ++it )
            printf("(%d,%d) to (%d,%d) => (%d,%d,%d,%d,%d,%d) \n",(*it).xEndPoints[0],(*it).yEndPoints[0],(*it).xEndPoints[1],(*it).yEndPoints[1],(*it).dy,(*it).dx, (*it).xOfyMin, (*it).xOfyMax, (*it).counter, (*it).plotX);
        //printf("\n");
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
        ////printf("%d : ",i);
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
 
void drawAndFill(float *x, float *y, int vertCount)
{
	SortedEdgeTable MainTable;
    int maxY=0,minY=4000,minX=4000,maxX=0;
	glColor3f(0.0f,1.0f,0.0f);
	for(int i=0;i<vertCount;i++){
        MainTable.addTuple((int)x[i], (int)y[i],(int) x[(i+1)%vertCount],(int) y[(i+1)%vertCount]);
        if(minY>y[i])minY=y[i];
        if(maxY<y[i])maxY=y[i];
        if(minX>x[i])minX=x[i];
        if(maxX<x[i])maxX=x[i];
	}
	MainTable.sortTable();
	scanlineFill(&MainTable, minY, maxY, 0, 1, 0);
}
/*
void drawAndFillVer1()
{
	glColor3f(0.0f,1.0f,0.0f);
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
	glFlush();
}*/

int xC=50,yC=50,rad=50;

//void fillCircle(/*int xC, int yC, int rad*/)
/*{
    glClear(GL_COLOR_BUFFER_BIT);
    for(int y=yC-rad;y<=yC+rad;y++)
    {
        for(int x=xC-rad;x!=xC+rad;x++)
            if((x-xC)*(x-xC)+(y-yC)*(y-yC)-rad*rad < 0)
            {
                //printf("f");
                double chordLength=2*sqrt(y*(2*rad-y));
                glBegin(GL_LINES);
                    glVertex2d((double)x,(double)y);
                    glVertex2d(x+chordLength,(double)y);
                glEnd();
                x=xC+rad;
            }
    }
    glFlush();
}
*/
void Piston();

float x1, y1, x2, y2,x3,y3,x4,y4;
float vert_x[]={130, 130, 160, 160, 130, 130, 270, 270 ,240 ,240 ,270 ,270};
float vert_y[]={460, 440, 440, 430, 430, 300, 300, 430, 430, 440, 440, 460};
float vert_outx[]={110,110,290,290};
float vert_outy[]={320,720,720,320};
float vert_inx[]={130,130,270,270};
float vert_iny[]={310,700,700,310};
float circle[]={200,380};
float trans_point[]={circle[0]-20,circle[1],circle[0]+20,circle[1]};
float circle_rot[]={200,125};
float **ax4 ;
float X, Y, X1, Y1;
float count1 = 0,y_coordinate ,translate = 13.33 ;
double angle = 9*3.14/180 ;
double angle1 = 0 ;
void myInit() {
glClear(GL_COLOR_BUFFER_BIT);
glClearColor(1.0, 0.9, 0.8, 1.0);
glMatrixMode(GL_PROJECTION);
gluOrtho2D(-500, 1500,-200, 1000);
}

void draw_pixel(int x, int y) {
glColor3f(1.0,0.0,0.0) ;
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();
}
void drawcircle(float a,float b,float c)
{
    if(c==150){
        //printf("Here!\n");
    }
    draw_pixel(a,b);
    float x=0;
    float y=c;
    float h=1-c;
    while(y>x){
        if(h<0)
            h=h+(2*x)+3;
        else{
            h=h+(2*(x-y))+5;
            y--;
        }
        x++;
        draw_pixel(a+x,b+y);
        draw_pixel(a+x,b-y);
        draw_pixel(a-x,b+y);
        draw_pixel(a-x,b-y);
        draw_pixel(a+y,b+x);
        draw_pixel(a+y,b-x);
        draw_pixel(a-y,b+x);
        draw_pixel(a-y,b-x);
    }
    glFlush();
}
void draw_line(float x1, float x2, float y1, float y2) {
    float dx, dy, i, e;
    float incx, incy, inc1, inc2;
    float x,y;
    dx = x2-x1;
    dy = y2-y1;
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    incx = 1;
    if (x2 < x1)
        incx = -1;
    incy = 1;
    if (y2 < y1)
        incy = -1;
    x = x1;
    y = y1;
    if (dx > dy) {
        draw_pixel(x, y);
        e = 2 * dy-dx;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;
        for (i=0; i<dx; i++) {
            if (e >= 0) {
                y += incy;
                e += inc1;
            }
            else
                e += inc2;
            x += incx;
            draw_pixel(x, y);
        }
    }
    else {
        draw_pixel(x, y);
        e = 2*dx-dy;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;
        for (i=0; i<dy; i++){
            if (e >= 0) {
                x += incx;
                e += inc1;
            }
            else
                e += inc2;
                y += incy;
                draw_pixel(x, y);
        }
    }
    count1++ ;

}
void Piston()
{

    //Drawing Piston
    for(int i=0;i<11;i++){
            draw_line(vert_x[i],vert_x[i+1],vert_y[i],vert_y[i+1]);

    }
    draw_line(vert_x[11],vert_x[0],vert_y[11],vert_y[0]) ;
   // float vert_x[]={130, 130, 160, 160, 130, 130, 270, 270 ,240 ,240 ,270 ,270};
   //float vert_y[]=    {460, 440, 440, 430, 430, 300, 300, 430, 430, 440, 440, 460};
   /* glBegin(GL_POLYGON);
        glColor3f(0,1,0);
        glVertex2f(130,460);
        glVertex2f(130,440);
        glVertex2f(160,440);
        glVertex2f(160,430);
        glVertex2f(130,430);
        glVertex2f(130,300);
        glVertex2f(270,300);
        glVertex2f(270,430);
        glVertex2f(240,430);
        glVertex2f(240,440);
        glVertex2f(270,440);
        glVertex2f(270,460);

    glEnd();*/

    //drawAndFill(vert_x,vert_y,12);

    //Drawing upper circles (Two Circle)
    drawcircle(circle[0],circle[1],15);
    drawcircle(circle[0],circle[1],20);

    //Drawing upper translating circle
    drawcircle(circle_rot[0],circle_rot[1],55);

    //Drawing crankshaft
    draw_line(trans_point[0],circle_rot[0]-55,trans_point[1],circle_rot[1]);
    draw_line(trans_point[2],circle_rot[0]+55,trans_point[3],circle_rot[1]);



    //Translating condition
    long long int ab = 1000000;
    while(ab--){;}
    if( vert_y[11] >= 700 ){
        translate = -10 ;
    }
    else if( vert_y[0] <= 500 ){
        translate = 10 ;
    }

    //Translating Piston
    for(int i = 0 ; i < 12 ; i++){
        ax4 = TranslateMatrix(0,translate,&vert_x[i],&vert_y[i]) ;
        //cout << "a[0]" << ax4[0][0] << " " << ax4[1][0] << endl ;
        vert_x[i] = ax4[0][0] ;
        vert_y[i] = ax4[1][0] ;
    }

    //Translating upper circle
    ax4 = TranslateMatrix(0,translate,&circle[0],&circle[1]) ;
    circle[0]=ax4[0][0];
    circle[1]=ax4[1][0];

    //Rotating lower circle
    ax4 = Rotation(200,200,angle,&circle_rot[0],&circle_rot[1]);
    circle_rot[0]=ax4[0][0];
    circle_rot[1]=ax4[1][0];

    //Translating upper points
    ax4 = TranslateMatrix(0,translate,&trans_point[0],&trans_point[1]);
    trans_point[0]=ax4[0][0];
    trans_point[1]=ax4[1][0];
    ax4 = TranslateMatrix(0,translate,&trans_point[2],&trans_point[3]);
    trans_point[2]=ax4[0][0];
    trans_point[3]=ax4[1][0];

    //Rotating Lower points
//    ax4 = Rotation(200,200,angle,&rot_point[0],&rot_point[1]);
//    rot_point[0]=ax4[0][0];
//    rot_point[1]=ax4[1][0];
//    ax4 = Rotation(200,200,angle,&rot_point[2],&rot_point[3]);
//    rot_point[2]=ax4[0][0];
//    rot_point[3]=ax4[1][0];

    glClear(GL_COLOR_BUFFER_BIT);

}
void myDisplay() {

    //Drawing lower fixed Circles
    drawcircle(200,200,30);
    drawcircle(200,200,40);
    drawcircle(200,200,150);
    drawcircle(200,200,130 );

    //Drawing the outer case
    //cout << vert_y[0] << " " << vert_y[11] << " " << y_coordinate << endl ;
     for(int i=0;i<3;i++)
    {
            draw_line(vert_outx[i],vert_outx[i+1],vert_outy[i],vert_outy[i+1]);
    }
     for(int i=0;i<3;i++)
    {
            draw_line(vert_inx[i],vert_inx[i+1],vert_iny[i],vert_iny[i+1]);
    }

    draw_line(trans_point[0],circle_rot[0]-55,trans_point[1],circle_rot[1]);
    draw_line(trans_point[2],circle_rot[0]+55,trans_point[3],circle_rot[1]);
    //cout << "here" << endl ;

//    glutSwapBuffers();
    Piston();


}
int main(int argc, char **argv) {
    //printf( "Enter (x1, y1, x2, y2)\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1470, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Bresenham's Line Drawing");
    myInit();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myDisplay) ;
    glutMainLoop();
    return(0);
}

/*
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

void mergeSortBucket(vector<Tuple>&left, vector<Tuple>& right, vector<Tuple>& bars)
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

void sortBucket(vector<Tuple> & bar)
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
*/

/*
int main()
{
    Tuple t;
    vector<Tuple> vect;
    t.xOfyMin=10;
    vect.push_back(t);
    t.xOfyMin=5;
    vect.push_back(t);
    t.xOfyMin=12;
    vect.push_back(t);
    t.xOfyMin=30;
    vect.push_back(t);
    t.xOfyMin=-30;
    vect.push_back(t);
    map<int,vector<Tuple>> buckets;
    buckets.insert(pair<int,vector<Tuple>>(0,vect));
    vector<Tuple> * vp = &buckets.at(0);
    for(vector<Tuple>::iterator it=(*vp).begin(); it!=(*vp).end(); ++it )
            (*it).xOfyMin=0;
    vp = &buckets.at(0);
    for(vector<Tuple>::iterator it=(*vp).begin(); it!=(*vp).end(); ++it )
            //cout<<(*it).xOfyMin;
/*
    map<int,vector<Tuple>> buckets;
    vector<Tuple> temp1,*temp2;
    Tuple t;
    t.yMax=10;
    temp1.push_back(t);
    t.yMax=20;
    temp1.push_back(t);
    buckets.insert(pair<int,vector<Tuple>>(0,temp1));
    temp2=&buckets.at(0);
    (*temp2)[0].yMax=-30;
    vector<Tuple>::iterator it=temp1.begin();
    //temp1.erase(it);
    t.yMax=40;
    buckets.at(0).push_back(t);
    //cout<<(buckets[0])[2].yMax;

    for( map<int,vector<Tuple>>::const_iterator mapIt = buckets.begin(); mapIt != buckets.end(); ++mapIt )
    {
        int key = (*mapIt).first;
        vector<Tuple> vect = (*mapIt).second;
        //printf("Scanline(yMin) %d : \n",key);
        for(vector<Tuple>::iterator it=vect.begin(); it!=vect.end(); ++it )
            //printf("%d \n",(*it).yMax);
        //printf("\n");
    }
}*/



