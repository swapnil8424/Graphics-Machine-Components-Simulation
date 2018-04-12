#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

int vert_x[]={130, 130, 160, 160, 130, 130, 270, 270 ,240 ,240 ,270 ,270};
int vert_y[]={460, 440, 440, 430, 430, 300, 300, 430, 430, 440, 440, 460};
int vert_outx[]={110,110,290,290};
int vert_outy[]={320,720,720,320};
int vert_inx[]={130,130,270,270};
int vert_iny[]={310,700,700,310};
int n=12;
int m=4;

void plotpoint(int x,int y)
{
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    printf("%d  %d\n",x,y);
    glEnd();
}

void myInit (void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(3.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 800.0);
}

void horizontal(int x1,int y1,int x2,int y2)
{
    if(x1>x2)
    {
        float temp;
        temp = x1;
        x1 = x2;
        x2 = temp;
    }
    for(float x=x1; x<=x2; x++)
    {
        plotpoint(x,y1);
    }
}

void vertical(int x1,int y1,int x2,int y2)
{
    if(y1>y2)
    {
        float temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    for(float y=y1; y<=y2; y++)
    {
        plotpoint(x1,y);
    }
}

void bressenhams(int x1,int y1,int x2,int y2)
{
    printf("%d %d %d %d\n",x1,y1,x2,y2);
    if(x1>x2)
    {
        float temp;
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int x = x1, y = y1;
    int dx = x2-x1;
    int dy = y2-y1;
    int dT = 2*(dy-dx);
    int dS = 2*dy;
    int d = 2*dy-dx;

    plotpoint(x,y);
    while(x<x2)
    {
        x++;
        if(d<0)
        {
            d = d+dS;
        }
        else
        {
            d = d+dT;
            y++;
        }
        plotpoint(x,y);
    }
    plotpoint(x2,y2);
    //glFlush();
}

void drawcircle(int a,int b,int c)
{
    if(c==150)
    {
        printf("Here!\n");
    }
    plotpoint(a,b);
    int x=0;
    int y=c;
    int h=1-c;
    while(y>x)
    {
        if(h<0)
            h=h+(2*x)+3;
        else
        {
            h=h+(2*(x-y))+5;
            y--;
        }
        x++;
        plotpoint(a+x,b+y);
        plotpoint(a+x,b-y);
        plotpoint(a-x,b+y);
        plotpoint(a-x,b-y);
        plotpoint(a+y,b+x);
        plotpoint(a+y,b-x);
        plotpoint(a-y,b+x);
        plotpoint(a-y,b-x);
    }
}
void display()
{
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(1.0,0.0,0.0);
    for(int i=0;i<n-1;i++)
    {
        if(vert_y[i]==vert_y[i+1])
            horizontal(vert_x[i],vert_y[i],vert_x[i+1],vert_y[i+1]);
        else if(vert_x[i]==vert_x[i+1])
            vertical(vert_x[i],vert_y[i],vert_x[i+1],vert_y[i+1]);
        else
            bressenhams(vert_x[i],vert_y[i],vert_x[i+1],vert_y[i+1]);
    }
    bressenhams(vert_x[0],vert_y[0],vert_x[n-1],vert_y[n-1]);
    drawcircle(275,210,40);
    drawcircle(200,200,30);
    drawcircle(200,200,40);
    drawcircle(200,200,150);
    drawcircle(200,200,130 );
    drawcircle(200,380,15);
    drawcircle(200,380,20);
    bressenhams(0,0,200,800);


    for(int i=0;i<m-1;i++)
  {
        if(vert_outy[i]==vert_outy[i+1])
            horizontal(vert_outx[i],vert_outy[i],vert_outx[i+1],vert_outy[i+1]);
        else if(vert_outx[i]==vert_outx[i+1])
            vertical(vert_outx[i],vert_outy[i],vert_outx[i+1],vert_outy[i+1]);
        else
            bressenhams(vert_outx[i],vert_outy[i],vert_outx[i+1],vert_outy[i+1]);
    }
    //bressenhams(vert_outx[0],vert_outy[0],vert_outx[m-1],vert_outy[m-1]);
        for(int i=0;i<m-1;i++)
  {
        if(vert_iny[i]==vert_iny[i+1])
            horizontal(vert_inx[i],vert_iny[i],vert_inx[i+1],vert_iny[i+1]);
        else if(vert_inx[i]==vert_inx[i+1])
            vertical(vert_inx[i],vert_iny[i],vert_inx[i+1],vert_iny[i+1]);
        else
            bressenhams(vert_inx[i],vert_iny[i],vert_inx[i+1],vert_iny[i+1]);
    }
    //bressenhams(vert_outx[0],vert_outy[0],vert_outx[m-1],vert_outy[m-1]);

    drawcircle(275,210,55);
    bressenhams(229,240,185,367);
    bressenhams(321,240,215,367);



    glFlush();
    glutSwapBuffers();
}


int main(int argc,char **argv)
{
//    printf("Enter the number of vertices\n");
//    scanf("%d",&n);
//    for(int j=0;j<n;j++)
//    {
//        scanf("%d%d",&vert_x[j],&vert_y[j]);
//    }
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(1024,1024);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Polygon Draw");
	glutDisplayFunc(display);
	myInit();
	glutMainLoop();

}
