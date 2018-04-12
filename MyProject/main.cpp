#include<windows.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include "header.h"
using namespace std;

float x1, y1, x2, y2,x3,y3,x4,y4;
float X, Y, X1, Y1;
float count1 = 0,y_coordinate ,translate =20 ;
double angle = 5*3.14/180 ;
double angle1 = 0 ;
void myInit() {
glClear(GL_COLOR_BUFFER_BIT);
glClearColor(1.0, 0.9, 0.8, 1.0);
glMatrixMode(GL_PROJECTION);
gluOrtho2D(0, 500, 0, 500);
}
void draw_pixel(int x, int y) {
glColor3f(1.0,0.0,0.0) ;
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();
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
draw_line(x1, x2, y1, y2);
draw_line(x2, x3, y2, y3);
draw_line(x3, x4, y3, y4);
draw_line(x4, x1, y4, y1);
cout << y1 << " " << y3 << " " << y_coordinate << endl ;
glClear(GL_COLOR_BUFFER_BIT);
    float **ax ;
    long long int ab = 100000000;
    while(ab--){;}
    if( y3 >= 500 ){
        translate = -20 ;
    }
    else if( y1 <= 20 ){
        translate = 20 ;
    }
    float **ax4 ;
    cout << "here" << endl ;
    ax4 = TranslateMatrix(0,translate,&x1,&y1) ;
    angle1 += angle ;
    x1 = ax4[0][0] ;
    y1 = ax4[1][0] ;
    ax = TranslateMatrix(0,translate,&x2,&y2) ;
    angle1 += angle ;
    x2 = ax[0][0] ;
    y2 = ax[1][0] ;
    float **ax1 ;
    ax1 = TranslateMatrix(0,translate,&x3,&y3) ;
    angle1 += angle ;
    x3 = ax1[0][0] ;
    y3 = ax1[1][0] ;
    float **ax2 ;
    ax2 = TranslateMatrix(0,translate,&x4,&y4) ;
    angle1 += angle ;
    x4 = ax2[0][0] ;
    y4 = ax2[1][0] ;
    y_coordinate += 20 ;

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
//int a,b;
//int x0,y00;
//double angle = 0 ;
//int i = 0 ;
//int temp =0 ;
//
//float m;
//
//
//void setPixel (GLint xCoord, GLint yCoord)
//{ //glLoadIdentity();
////glTranslatef(x0,y00,0);
// glBegin (GL_POINTS);
// glVertex2i (xCoord, yCoord);
// glEnd();
//}
//void Drawcircle(int x,int y)
//{
//    setPixel(x+x0,y+y00);
//    setPixel(-1*x+x0,-1*y+y00);
//    setPixel(x+x0,-1*y+y00);
//    setPixel(-1*x+x0,y+y00);
//
//
//}
//void swap(int *x,int *y)
//{int temp;
//    temp=*x;
//    *x=*y;
//    *y=temp;
//}
//void ellipse ()
//{   int x=temp ;
//   int y=b;
//    int d=b*b+a*a*(0.25-b);
//     //glLoadIdentity();
//     //glTranslatef(x0,y00,0);
//   while(b*b*(x+1)<a*a*(y-0.5))
//   { Drawcircle(x,y);
//    if (d>=0)
//     {
//        d=d+a*a*(2-2*y)+(2*x+3)*b*b;
//        y=y-1;
//
//     }
//     else
//      {d=d+(2*x+3)*b*b;}
//     x=x+1;
//   }
//   d=b*b*(x+0.5)*(x+0.5) +a*a*(y-1)*(y-1) - a*a*b*b ;
//while(y>=0)
//   { Drawcircle(x,y);
//    if (d>=0)
//     {
//        d=d+a*a*(3-2*y);
//
//     }
//     else
//      {d=d+(2*x+2)*b*b+a*a*(3-2*y);
//    x=x+1;}
//     y=y-1;
//   }
////x0+=0.01;
//
//}
//void rotate_ellipse(){
//    ellipse();
//    glFlush();
//    int **an = Rotation(x0,y00,angle + i*3.14/180,&temp,&b) ;
//    temp = an[0][0];
//    b = an[1][0] ;
//    i += 10 ;
//    int l = 1000000000;
//    while(l--){;}
//}
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
