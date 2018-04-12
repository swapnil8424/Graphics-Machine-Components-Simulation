#include<bits/stdc++.h>
#include<iostream>
#include<math.h>
#include<cstdio>
#include "header.h"

using namespace std ;

float **Rotation(float X, float Y,double angle,float *x,float *y){
    float **t1 = TranslateMatrix(-X,-Y,x,y);

    float RotationMatrix[3][3] = { { (float)cos(angle) , (float)(-1*sin(angle)), 0 },
                         { (float)sin(angle) , (float)cos(angle) , 0               },
                         {             0     ,         0         ,               1 }
                       };

    float **PofloatMatrix ;
    PofloatMatrix = new float*[3] ;
    PofloatMatrix[0] = new float[1] ;
    PofloatMatrix[1] = new float[1] ;
    PofloatMatrix[2] = new float[1] ;
    float w = 1;
    float *W = &w ;
    float z = t1[0][0] ;
    float z1 = t1[1][0] ;
//    cout << RotationMatrix[0][1] << " " << RotationMatrix[1][0] << " " << RotationMatrix[2][2] << endl ;
	PofloatMatrix[0][0] = z ;
	PofloatMatrix[1][0] = z1 ;
	PofloatMatrix[2][0] = *W ;
//	cout << PofloatMatrix[0][0] << " " << PofloatMatrix[1][0] << " " << PofloatMatrix[2][0] << endl ;

	float temp[3][1] = {{0},{0},{0}};
    //Multiplication
    float Sum = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            Sum += (RotationMatrix[i][j])*(PofloatMatrix[j][0]) ;
        }
        temp[i][0] = Sum ;
        Sum = 0 ;
    }

    float j1 = temp[0][0] ;
    float j2 = temp[1][0] ;
    PofloatMatrix = TranslateMatrix(X,Y,&j1,&j2) ;

    return PofloatMatrix ;
}

float **TranslateMatrix(float X,float Y,float *x, float *y){
    float TranslateMatrix[3][3] = { { 1 , 0 , X } ,
                                   { 0 , 1 , Y } ,
                                   { 0 , 0 , 1 }
                                 };
    float **PofloatMatrix ;
    PofloatMatrix = new float*[3] ;
    PofloatMatrix[0] = new float[1] ;
    PofloatMatrix[1] = new float[1] ;
    PofloatMatrix[2] = new float[1] ;
    float w = 1;
    float *W = &w ;
    //cout << RotationMatrix[0][1] << " " << RotationMatrix[1][0] << " " << RotationMatrix[2][2] << endl ;
	PofloatMatrix[0][0] = *x ;
	PofloatMatrix[1][0] = *y ;
	PofloatMatrix[2][0] = *W ;

    float temp[3][1] = {{0},{0},{0}};
    //Multiplication
    float Sum  = 0.0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            Sum += (TranslateMatrix[i][j])*(PofloatMatrix[j][0]) ;
        }
//        cout << endl;
        temp[i][0] = Sum ;
        Sum = 0 ;
    }
    PofloatMatrix[0][0] = temp[0][0] ;
    PofloatMatrix[1][0] = temp[1][0] ;
    PofloatMatrix[2][0] = temp[2][0] ;

    return PofloatMatrix ;
}

float **ScalingMatrix(float Sx,float Sy,float *x,float *y){
		float ScalingMatrix[3][3] = { { Sx , 0  , 0 } ,
                                    { 0  , Sy , 0 } ,
                                    { 0  , 0  , 1 }
                                  };
    float **PofloatMatrix ;
    PofloatMatrix = new float*[3] ;
    PofloatMatrix[0] = new float[1] ;
    PofloatMatrix[1] = new float[1] ;
    PofloatMatrix[2] = new float[1] ;
    float w = 1;
    float *W = &w ;
//    cout << ScalingMatrix[0][1] << " a " << ScalingMatrix[1][0] << " a " << ScalingMatrix[2][2] << endl ;
	PofloatMatrix[0][0] = *x ;
	PofloatMatrix[1][0] = *y ;
	PofloatMatrix[2][0] = *W ;

//    cout << PofloatMatrix[0][0] << " b " << PofloatMatrix[1][0] << " b " << PofloatMatrix[2][0] << endl ;


    float temp[3][1] = {{0},{0},{0}};
    //Multiplication
//    cout << ScalingMatrix[0][0]*PofloatMatrix[0][0] << " loo " <<endl ;
    float Sum = 0 ;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            Sum += (ScalingMatrix[i][j])*(PofloatMatrix[j][0]) ;
//            if( i == 0 ){
//                cout << ScalingMatrix[0] << endl ;
//            }
        }
//        cout << endl;
        temp[i][0] = Sum ;
        Sum = 0 ;
    }
    PofloatMatrix[0][0] = temp[0][0] ;
//    cout << "Manas are:" << temp[0][0] << endl ;
    PofloatMatrix[1][0] = temp[1][0] ;
    PofloatMatrix[2][0] = temp[2][0] ;

    return PofloatMatrix ;
	}

//float main(){
//		float x = 1, y = 1;
//		float *l = &x ;
//		float *m = &y ;
//		cout << (float)sin(90*3.14159/180) << endl ;
// 		float **a = TranslateMatrix( 3 , 3 , l ,m ) ;
// 		for(float i = 0 ;i < 3 ; i++){
//			cout <<  a[i][0] << endl;
//			}
//	}

