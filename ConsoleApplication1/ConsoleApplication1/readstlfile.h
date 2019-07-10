
#ifndef READSTLFILE_H
#define READSTLFILE_H
#include<vector>
#include"spoint3f.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
class ReadSTLFile
{
public:
    int ReadFile(const char *cfilename);
      int NumTri();
      vector<SPoint3f>& PointList();
      ReadSTLFile();
      void mshow();
	  void mchange();
	  vector<SPoint3f> pointList;
	  double min_x, min_y, min_z;
	  double max_x, max_y, max_z;
	  double k_xy;
	  double k_xz;
  protected:
        unsigned int unTriangles;
        bool ReadASCII(const char *cfilename);
        bool ReadBinary(const char *cfilename);
        char* memwriter;
        int cpyint(const char*& p);
        float cpyfloat(const char*& p);
};

#endif // READSTLFILE_H
