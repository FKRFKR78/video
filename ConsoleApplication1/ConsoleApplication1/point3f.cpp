#include "spoint3f.h"

SPoint3f::SPoint3f()
{

}
SPoint3f::SPoint3f(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}
int SPoint3f::SetParam(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
    return 0;
}

