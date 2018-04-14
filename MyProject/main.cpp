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
void myDisplay() {

    //Drawing lower fixed Circles
    drawcircle(200,200,30);
    drawcircle(200,200,40);
    drawcircle(200,200,150);
    drawcircle(200,200,130 );

    //Drawing the outer case
    cout << vert_y[0] << " " << vert_y[11] << " " << y_coordinate << endl ;
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
    cout << "here" << endl ;

//    glutSwapBuffers();
    Piston();


}
int main(int argc, char **argv) {
    printf( "Enter (x1, y1, x2, y2)\n");
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

