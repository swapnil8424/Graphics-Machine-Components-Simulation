#include<windows.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include "header.h"
#include<math.h>
using namespace std;

void Piston();
void scanfill(float *,float *,int,float,float,float);
//int xC=50,yC=50,rad=200;
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
long long int ab ;
void myInit() {
glClear(GL_COLOR_BUFFER_BIT);
glClearColor(1.0, 1.0, 1.0, 0.5);
glMatrixMode(GL_PROJECTION);
gluOrtho2D(-1000, 1800,-1300, 1800);
}

void fillCircle(int xC, int yC, int rad, float r, float g, float b)
{
//    glClear(GL_COLOR_BUFFER_BIT);
    for(int y=yC-rad;y<=yC+rad;y++)
    {
        for(int x=xC-rad;x!=xC+rad;x++)
            if((x-xC)*(x-xC)+(y-yC)*(y-yC)-rad*rad < 0)
            {
                //float chordLength=2*sqrt(y*(2*rad-y));
                glBegin(GL_POINTS);
                    glColor3f(r,g,b);
                    glVertex2i(x,y);
                    //glVertex2f(x+chordLength,(float)y);
                glEnd();
                //x=xC+rad;
            }
    }
    glFlush();

}

void draw_pixel(int x, int y) {
glColor3f(0.0,0.0,0.0) ;
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();
}
void drawcircle(float a,float b,float c)
{
    if(c==150){
        printf("Here!\n");
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
void Piston(){

    //Drawing Piston
    for(int i=0;i<11;i++){
            draw_line(vert_x[i],vert_x[i+1],vert_y[i],vert_y[i+1]);
    }
    draw_line(vert_x[11],vert_x[0],vert_y[11],vert_y[0]) ;

    scanfill(vert_x,vert_y,12,1.0,1.0,1.0);
    //drawAndFill(vert_x,vert_y,0.5,0.5,0.5);

    //Drawing upper circles (Two Circle)
    drawcircle(circle[0],circle[1],15);
    drawcircle(circle[0],circle[1],20);

    //Drawing upper translating circle
    drawcircle(circle_rot[0],circle_rot[1],55);
    fillCircle(circle_rot[0],circle_rot[1],55,0.0,0.0,0.0);

    //Drawing crankshaft
    draw_line(trans_point[0],circle_rot[0]-55,trans_point[1],circle_rot[1]);
    draw_line(trans_point[2],circle_rot[0]+55,trans_point[3],circle_rot[1]);
    //Translating condition
    ab = 1000000;
    //while(ab--){;}
    if( vert_y[11] >= 700 ){
        translate = -10 ;
    }
    else if( vert_y[0] <= 500 ){
        translate = 10 ;
    }

    //Translating Piston
    for(int i = 0 ; i < 12 ; i++){
        ax4 = TranslateMatrix(0,translate,&vert_x[i],&vert_y[i]) ;
        cout << "a[0]" << ax4[0][0] << " " << ax4[1][0] << endl ;
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
void draw_pixel1(int x,int y,float r,float g,float b)
{
    glColor3f(r,g,b);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
}

void edgedetect(float x1,float y1,float x2,float y2,int *le,int *re)
{
    float temp,x,mx;
    int i;

    if(y1>y2)
    {
        temp=x1,x1=x2,x2=temp;
        temp=y1,y1=y2,y2=temp;
    }

    if(y1==y2)
        mx=x2-x1;
    else
        mx=(x2-x1)/(y2-y1);

    x=x1;

    for(i=int(y1);i<=(int)y2;i++)
    {
        if(x<(float)le[i]) le[i]=(int)x;
        if(x>(float)re[i]) re[i]=(int)x;
        x+=mx;
    }
}

void scanfill(float *x_vert,float *y_vert,int n,float r,float g,float b)
{
    int le[10000],re[10000],i,j;

    for(int i=0;i<10000;i++)
        le[i]=10000,re[i]=0;
    for(int i=0;i<n-1;i++){
        edgedetect(x_vert[i],y_vert[i],x_vert[i+1],y_vert[i+1],le,re);
    }

    edgedetect(x_vert[n-1],y_vert[n-1],x_vert[0],y_vert[0],le,re);


    for(j=0;j<5000;j++)
    {
        if(le[j]<=re[j])
            for(i=le[j];i<re[j];i++)
                draw_pixel1(i,j,r,g,b);
    }

}




void myDisplay() {


    //draw_line(-1000,1800,1400,1400);
    //draw_line(-1000,1800,-900,-900);

//    float grass_x[]={-1000,-1000,1800,1800};
//    float grass_y[]={1400,1800,1800,1400};
//    float grass_x1[]={-1000,-1000,1800,1800};
//    float grass_y1[]={-1300,-900,-900,-1300};


    //Drawing the outer case
    Piston();
    cout << vert_y[0] << " " << vert_y[11] << " " << y_coordinate << endl ;
     for(int i=0;i<3;i++)
    {
            draw_line(vert_outx[i],vert_outx[i+1],vert_outy[i],vert_outy[i+1]);
    }
    scanfill(vert_outx,vert_outy,4,1.0,0,0);
     for(int i=0;i<3;i++)
    {
            draw_line(vert_inx[i],vert_inx[i+1],vert_iny[i],vert_iny[i+1]);
    }
    scanfill(vert_inx,vert_iny,4,0,0,0);

    //Drawing lower fixed Circles

    fillCircle(200,200,150,0.4,0.4,0.3) ;
    fillCircle(200,200,130,1,0.4,0.2);
    fillCircle(200,200,40,1,0.5,0.5);
    fillCircle(200,200,30,0.5,0.5,1);

    //Drawing right center circle ;
    fillCircle(1100,200,70,0,1,0) ;
    fillCircle(1100,200,40,0,0,1) ;

    //Drawing connecting line
    draw_line(200,1100,240,270) ;
    draw_line(200,1100,160,130) ;


   //Drawing right bottom circle
    fillCircle(1100,-600,130,0.0,0.0,0.0) ;
    fillCircle(1100,-600,80,1,1,1) ;

    //Drawing right top circle
    fillCircle(1100,1000,130,0.0,0.0,0.0) ;
    fillCircle(1100,1000,80,1.0,1.0,1.0) ;


    //Drawing line connecting tires
    draw_line(1080,1030,925,200);
    draw_line(1030,1080,200,-515);
    draw_line(1120,1170,925,200);
    draw_line(1170,1120,200,-515);
    //draw_line(1080,1080,-515,925);
    //draw_line(1120,1120,-515,925) ;

    //drawing rims

    draw_line(1100,1100,1080,920);
    draw_line(1020,1180,1000,1000);

    draw_line(1100,1100,-520,-680);
    draw_line(1020,1180,-600,-600);


    fillCircle(-300,600,100,0.0,0.0,0.0);
    fillCircle(-300,600,60,1.0,1.0,1.0);
    fillCircle(-300,-200,100,0.0,0.0,0.0);
    fillCircle(-300,-200,60,1.0,1.0,1.0);

    //drawing body
    draw_line(-650,-650,300,100);
    draw_line(-650,-200,300,500);
    draw_line(-650,-200,100,-100);

    draw_line(-200,-150,500,800);
    draw_line(-200,-150,-100,-400);

    draw_line(-150,1100,800,1000);
    draw_line(-150,1100,-400,-600);

    draw_line(1230,1500,1000,1000);
    draw_line(1230,1500,-600,-600);
    draw_line(1500,1500,1000,700);
    draw_line(1500,1400,700,700);
    draw_line(1400,1400,700,-300);
    draw_line(1400,1500,-300,-300);
    draw_line(1500,1500,-300,-600);

    //coloring body
//    float body_x[]={-650,-200,-150,1100,1230,1500,1500,1400,1400,1500,1500,1230,1100,-150,-200,-650};
//    float body_y[]={300,500,800,1000,1000,1000,700,700,-300,-300,-600,-600,-600,-400,-100,100};
   //scanfill1(body_x,body_y,16,0.0,1.0,0.0);

    //drawing road
//    glBegin(GL_POLYGON);
//        glColor3f(0.6,0.6,0.6);
//        glVertex2f(-1000,1400);
//        glVertex2f(1800,1400);
//        glVertex2f(1800,-900);
//        glVertex2f(-1000,-900);
//    glEnd();

    glBegin(GL_POLYGON);
        glPointSize(10.0);
        glColor3f(0.0,1.0,0.8);
        glVertex2f(-1000,1400);
        glVertex2f(-1000,1800);
        glVertex2f(1800,1800);
        glVertex2f(1800,1400);
    glEnd();
    glBegin(GL_POLYGON);
    glPointSize(10.0);
        glColor3f(0.0,1.0,0.8);
        glVertex2f(-1000,-1300);
        glVertex2f(-1000,-900);
        glVertex2f(1800,-900);
        glVertex2f(1800,-1300);
    glEnd();

    //float grass_x1[]={-1000,-1000,1800,1800};
//    float grass_y1[]={-1300,-900,-900,-1300};


    draw_line(trans_point[0],circle_rot[0]-55,trans_point[1],circle_rot[1]);
    draw_line(trans_point[2],circle_rot[0]+55,trans_point[3],circle_rot[1]);
    float crank_x[]={trans_point[0],circle_rot[0]-55,circle_rot[0]+55,trans_point[2]};
    float crank_y[]={trans_point[1],circle_rot[1],circle_rot[1],trans_point[1]};
    scanfill(crank_x,crank_y,4,1.0,0.7,0.0);

//    glutSwapBuffers();



}
int main(int argc, char **argv) {
    printf( "Enter (x1, y1, x2, y2)\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1550, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Bresenham's Line Drawing");
    myInit();
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myDisplay) ;
    glutMainLoop();
    return(0);
}
