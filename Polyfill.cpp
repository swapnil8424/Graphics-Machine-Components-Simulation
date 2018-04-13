// CPP program to illustrate
// Scan-line Polygon fill Algorithm

#include<GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define maxHt 1080
#define maxWd 1920
#define maxVer 10000

FILE *fp;

typedef struct edgetuple
{
	int ymax;
	float xofymin;
	float slopeinverse;
}EdgeTuple;

typedef struct edgetablebucket
{
	int countEdgeTuple;
	EdgeTuple tuples[maxVer];
}EdgeTableBucket;

EdgeTableBucket EdgeTable[maxHt], ActiveEdgeBucket;

void initEdgeTable()
{
	for (int i=0; i<maxHt; i++)
		EdgeTable[i].countEdgeTuple = 0;
	ActiveEdgeBucket.countEdgeTuple = 0;
}

void insertionSort(EdgeTableBucket *ett)
{
	int i,j;
	EdgeTuple temp;
	for (i = 1; i < ett->countEdgeTuple; i++)
	{
		temp.ymax = ett->tuples[i].ymax;
		temp.xofymin = ett->tuples[i].xofymin;
		temp.slopeinverse = ett->tuples[i].slopeinverse;
		j = i - 1;
        while ((temp.xofymin < ett->tuples[j].xofymin) && (j >= 0))
        {
            ett->tuples[j + 1].ymax = ett->tuples[j].ymax;
            ett->tuples[j + 1].xofymin = ett->tuples[j].xofymin;
            ett->tuples[j + 1].slopeinverse = ett->tuples[j].slopeinverse;
            j = j - 1;
        }
        ett->tuples[j + 1].ymax = temp.ymax;
        ett->tuples[j + 1].xofymin = temp.xofymin;
        ett->tuples[j + 1].slopeinverse = temp.slopeinverse;
	}
}

void storeEdgeInBucket (EdgeTableBucket *receiver,int ym,int xm,float slopInv)
{
	(receiver->tuples[(receiver)->countEdgeTuple]).ymax = ym;
	(receiver->tuples[(receiver)->countEdgeTuple]).xofymin = (float)xm;
	(receiver->tuples[(receiver)->countEdgeTuple]).slopeinverse = slopInv;
	insertionSort(receiver);
	(receiver->countEdgeTuple)++;
}

void storeEdgeInTable (int x1,int y1, int x2, int y2)
{
	float m,minv;
	int ymaxTS,xwithyminTS, scanline;
	if (x2==x1)
		minv=0.000000;
	else
	{
        m = ((float)(y2-y1))/((float)(x2-x1));
        if (y2==y1)
            return;
        minv = (float)1.0/m;
       // printf("\nSlope string for %d %d & %d %d: %f",x1,y1,x2,y2,minv);
	}
	if (y1>y2)
	{
		scanline=y2;
		ymaxTS=y1;
		xwithyminTS=x2;
	}
	else
	{
		scanline=y1;
		ymaxTS=y2;
		xwithyminTS=x1;
	}
	storeEdgeInBucket(&EdgeTable[scanline],ymaxTS,xwithyminTS,minv);
}

void printTuple(EdgeTableBucket *tup)
{
	int j;

	if (tup->countEdgeTuple)
		printf("\nCount %d-----\n",tup->countEdgeTuple);

		for (j=0; j<tup->countEdgeTuple; j++)
		{
			printf(" %d+%.2f+%.2f",
			tup->tuples[j].ymax, tup->tuples[j].xofymin,tup->tuples[j].slopeinverse);
		}
}

void printTable()
{
	int i,j;

	for (i=0; i<maxHt; i++)
	{
		if (EdgeTable[i].countEdgeTuple)
			printf("\nScanline %d", i);

		printTuple(&EdgeTable[i]);
	}
}

void removeEdgeByYmax(EdgeTableBucket *Tup,int yy)
{
	int i,c;
	for (i=0; i< Tup->countEdgeTuple; i++)
		if (Tup->tuples[i].ymax == yy)
		{
			//printf("\nRemoved at %d",yy);
			for ( c = i ; c < Tup->countEdgeTuple -1 ; c++ )
            {
				Tup->tuples[c].ymax =Tup->tuples[c+1].ymax;
				Tup->tuples[c].xofymin =Tup->tuples[c+1].xofymin;
				Tup->tuples[c].slopeinverse = Tup->tuples[c+1].slopeinverse;
            }
            Tup->countEdgeTuple--;
			i--;
		}
}

void updatexbyslopeinv(EdgeTableBucket *Tup)
{
	int i;
	for (i=0; i<Tup->countEdgeTuple; i++)
		(Tup->tuples[i]).xofymin =(Tup->tuples[i]).xofymin + (Tup->tuples[i]).slopeinverse;
}

void ScanlineFill()
{
	/* Follow the following rules:
	1. Horizontal edges: Do not include in edge table
	2. Horizontal edges: Drawn either on the bottom or on the top.
	3. Vertices: If local max or min, then count twice, else count
		once.
	4. Either vertices at local minima or at local maxima are drawn.*/
	int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;
	for (i=0; i<maxHt; i++)
	{
		for (j=0; j<EdgeTable[i].countEdgeTuple; j++)
		{
			storeEdgeInBucket(&ActiveEdgeBucket,EdgeTable[i].tuples[j].
					ymax,EdgeTable[i].tuples[j].xofymin,
					EdgeTable[i].tuples[j].slopeinverse);
		}
		removeEdgeByYmax(&ActiveEdgeBucket, i);
		insertionSort(&ActiveEdgeBucket);
		j = 0;
		FillFlag = 0;
		coordCount = 0;
		x1 = 0;
		x2 = 0;
		ymax1 = 0;
		ymax2 = 0;
		while (j<ActiveEdgeBucket.countEdgeTuple)
		{
			if (coordCount%2==0)
			{
				x1 = (int)(ActiveEdgeBucket.tuples[j].xofymin);
				ymax1 = ActiveEdgeBucket.tuples[j].ymax;
				if (x1==x2)
				/* three cases can arrive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom
				*/
					/*if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
					{
					    printf("Entered here (%d,%d) and (%d,%d)\n",x1,ymax1,x2,ymax2);
						x2 = x1;
						ymax2 = ymax1;
					}
					else*/
                        coordCount++;
				else
                        coordCount++;
			}
			else
			{
				x2 = (int)ActiveEdgeBucket.tuples[j].xofymin;
				ymax2 = ActiveEdgeBucket.tuples[j].ymax;
				FillFlag = 0;
				// checking for intersection...
				if (x1==x2)
				/*three cases can arive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom
				*/
					/*if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
					{
					    printf("Entered here (%d,%d) and (%d,%d)\n",x1,ymax1,x2,ymax2);
						x1 = x2;
						ymax1 = ymax2;
					}
					else*/
					{
						coordCount++;
						FillFlag = 1;
					}
				else
				{
						coordCount++;
						FillFlag = 1;
				}
			if(FillFlag)
			{
				glColor3f(0.0f,0.7f,0.0f);
				glBegin(GL_LINES);
				glVertex2i(x1,i);
				glVertex2i(x2,i);
				glEnd();
				glFlush();
			}
		}
		j++;
	}
	updatexbyslopeinv(&ActiveEdgeBucket);
 }

}


void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,maxHt,0,maxWd);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawPolyDino()
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
		{
			fscanf(fp, "%d,%d", &x1, &y1);
		}
		else
		{

			fscanf(fp, "%d,%d", &x2, &y2);
			//printf("Edge (%d,%d) to (%d,%d)\n",x1,y1,x2,y2);
			//printf("\n%d,%d", x2, y2);
			glBegin(GL_LINES);
				glVertex2i( x1+100, y1);
				glVertex2i( x2+100, y2);
			glEnd();
			storeEdgeInTable(x1+100, y1, x2+100, y2);
		}
	}
}

void drawDino(void)
{
	initEdgeTable();
	drawPolyDino();
	//printTable();
	ScanlineFill();
}

int main(int argc, char** argv)
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
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Scanline Polyfill");
	myInit();
	glutDisplayFunc(drawDino);
	glutMainLoop();
	fclose(fp);
}
