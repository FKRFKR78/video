#ifndef POINT3F_H
#define POINT3F_H
#include<iostream>
#include<stdio.h>
#include<Windows.h>
struct Vertex //顶点结构
{
    Vertex() {}
	Vertex(double x, double y, double z)
    {
        _x = x;  _y = y;  _z = z;
    }
	double _x, _y, _z;
    static const DWORD FVF;
};
class SPoint3f
{
public:
    SPoint3f();
	SPoint3f(double _x, double _y, double _z);
	int SetParam(double _x, double _y, double _z);
       inline Vertex IVertex()
       {
           return Vertex(x, y, z);
       }
       double x, y, z;
};

#endif // POINT3F_H
