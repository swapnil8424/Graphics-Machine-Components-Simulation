#include<windows.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include "header.h"
using namespace std;

void Piston();

float x1, y1, x2, y2,x3,y3,x4,y4;
float vert_x[]={130, 130, 160, 160, 130, 130, 270, 270 ,240 ,240 ,270 ,270};
float vert_y[]={460, 440, 440, 430, 430, 300, 300, 430, 430, 440, 440, 460};
float **ax4 ;
float X, Y, X1, Y1;
float count1 = 0,y_coordinate ,translate = 10 ;
double angle = 5*3.14/180 ;
double angle1 = 0 ;
void myInit() {
glClear(GL_COLOR_BUFFER_BIT);
glClearColor(1.0, 0.9, 0.8, 1.0);
glMatrixMode(GL_PROJECTION);
gluOrtho2D(0, 640, 0, 800);
}

void draw_pixel(int x, int y) {
glColor3f(1.0,0.0,0.0) ;
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();
}
void drawcircle(float a,float b,float c)
{
    if(c==150)
    {
        printf("Here!\n");
    }
    draw_pixel(a,b);
    float x=0;
    float y=c;
    float h=1-c;
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
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;
    x = x1; y = y1;
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
    } else {
    draw_pixel(x, y);
    e = 2*dx-dy;
    inc1 = 2*(dx-dy);
    inc2 = 2*dx;
    for (i=0; i<dy; i++) {
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
//    cout << count1 << endl ;
glFlush();
}

void myDisplay() {

//draw_line(x1, x2, y1, y2);
//draw_line(x2, x3, y2, y3);
//draw_line(x3, x4, y3, y4);
//draw_line(x4, x1, y4, y1);
        for(int i=0;i<11;i++)
    {
            draw_line(vert_x[i],vert_x[i+1],vert_y[i],vert_y[i+1]);
    }
    draw_line(vert_x[11],vert_x[0],vert_y[11],vert_y[0]) ;
    drawcircle(275,210,40);
    drawcircle(200,200,30);
    drawcircle(200,200,40);
    drawcircle(200,200,150);
    drawcircle(200,200,130 );
    drawcircle(200,380,15);
    drawcircle(200,380,20);
    cout << vert_y[0] << " " << vert_y[11] << " " << y_coordinate << endl ;
    glClear(GL_COLOR_BUFFER_BIT);
    long long int ab = 10000;
    while(ab--){;}
    if( vert_y[11] >= 700 ){
        translate = -10 ;
    }
    else if( vert_y[0] <= 460 ){
        translate = 10 ;
    }
    cout << "here" << endl ;
    for(int i = 0 ; i < 12 ; i++){

        ax4 = TranslateMatrix(0,translate,&vert_x[i],&vert_y[i]) ;
        cout << "a[0]" << ax4[0][0] << " " << ax4[1][0] << endl ;
        vert_x[i] = ax4[0][0] ;
        vert_y[i] = ax4[1][0] ;
    }
//    ax = TranslateMatrix(0,translate,&x2,&y2) ;
//    angle1 += angle ;
//    x2 = ax[0][0] ;
//    y2 = ax[1][0] ;
//    float **ax1 ;
//    ax1 = TranslateMatrix(0,translate,&x3,&y3) ;
//    angle1 += angle ;
//    x3 = ax1[0][0] ;
//    y3 = ax1[1][0] ;
//    float **ax2 ;
//    ax2 = TranslateMatrix(0,translate,&x4,&y4) ;
//    angle1 += angle ;
//    x4 = ax2[0][0] ;
//    y4 = ax2[1][0] ;
    y_coordinate += 10 ;

}
int main(int argc, char **argv) {
printf( "Enter (x1, y1, x2, y2)\n");
x1 = 200; y1 = 20 ; x2 = 300 ; y2 = 20 ; x3 = 300;y3 =100;x4 = 200; y4 =100 ;
X =x1 ;
Y =y1 ;
X1 = x2 ;
Y1= y2 ;
y_coordinate = x1;
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
glutInitWindowSize(500, 500);
glutInitWindowPosition(0, 0);
glutCreateWindow("Bresenham's Line Drawing");
myInit();
glutDisplayFunc(myDisplay);
glutIdleFunc(myDisplay) ;
glutMainLoop();
return(0);
}
//#include <windows.h>
//#include <GL/glut.h>
//#include <stdlib.h>
//#include <math.h>
//#include<iostream>
//#include "header.h"
//using namespace std;
//
//void reshape(int w, int h)
//{
//glViewport(0.0, 0.0, w, h);
//}
//void intiopenGL()
//{
//glClearColor(1.0, 1.0, 1.0, 1.0);
//}
//int main(int argc, char** argv)
//{
//cout<<"enter centre point \n";
//    cin>>x0>>y00 ;
//cout<<"enter semi-minor and semi-major axis\n";
//cin>>b>>a;
//glutInit(&argc, argv);
//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
//glutInitWindowSize(800, 600);
//glutInitWindowPosition(0,0);
//glutCreateWindow("Ellipse algo");
//gluOrtho2D(-400,400,-300,300);//x-axis:-400-400 ; y-axis: -300-300
//intiopenGL();
//
//glutDisplayFunc(rotate_ellipse);
//glutIdleFunc(rotate_ellipse);
//glutReshapeFunc(reshape);
//glutMainLoop();
//return 0;
//}
