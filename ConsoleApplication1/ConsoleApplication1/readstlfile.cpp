#include "readstlfile.h"
ReadSTLFile::ReadSTLFile()
{
	min_x = 100.0;
	min_y = 100.0;
	min_z = 100.0;
	max_x = -100.0;
	max_y = -100.0;
	max_z = -100.0;
}
void ReadSTLFile::mchange()
{
	for (int i = 0; i<pointList.size(); i++)
	{
		pointList[i].x = pointList[i].x - min_x;
		pointList[i].y = pointList[i].y - min_y;
		pointList[i].z = pointList[i].z - min_z;
	}
	max_x = max_x - min_x; max_y = max_y - min_y; max_z = max_z - min_z;
	min_x = 0;min_y = 0;min_z = 0;
	k_xy = 500 / max(max_x,max_y);
	for (int i = 0; i<pointList.size(); i++)
	{
		pointList[i].x = pointList[i].x *k_xy;
		pointList[i].y = pointList[i].y *k_xy;
		pointList[i].z = pointList[i].z *k_xy;
	}
}
int ReadSTLFile::ReadFile(const char *cfilename)
{
	FILE * pFile;
	fopen_s(&pFile, cfilename, "rb");
	char *title[80];
	fread(title, sizeof(unsigned char), 80, pFile);
	int number;
	fread(&number, sizeof(int), 1, pFile);

	printf("%s\n", title);
	cout << number << endl;
	unTriangles = number;
	float unless[3];
	float p1[3];
	float p2[3];
	float p3[3];
	  int color;

	for (int i = 0; i < number; i++)
	{
		fread(&unless, sizeof(float), 3, pFile);
		fread(&p1, sizeof(float), 3, pFile);
		fread(&p2, sizeof(float), 3, pFile);
		fread(&p3, sizeof(float), 3, pFile);
	//	for (int i = 0; i < 3; i++)
		//{
			pointList.push_back(SPoint3f(p1[0], p1[1], p1[2]));
			pointList.push_back(SPoint3f(p2[0], p2[1], p2[2]));
			pointList.push_back(SPoint3f(p3[0], p3[1], p3[2]));
	//	}
			double sminx = min(min(p1[0], p2[0]), p3[0]);
			double sminy = min(min(p1[1], p2[1]), p3[1]);
			double sminz = min(min(p1[2], p2[2]), p3[2]);


			double smaxx = max(max(p1[0], p2[0]), p3[0]);
			double smaxy = max(max(p1[1], p2[1]), p3[1]);
			double smaxz = max(max(p1[2], p2[2]), p3[2]);
			min_x = min(sminx, min_x);
			min_y = min(sminy, min_y);
			min_z = min(sminz, min_z);
			max_x = max(smaxx, max_x);
			max_y = max(smaxy, max_y);
			max_z = max(smaxz, max_z);
		fread(&color, sizeof(char), 2, pFile);
	}

	fclose(pFile);
	return 1;
	/*
    FILE * pFile;
    long lSize;
    char* buffer;
    size_t result;

    fopen_s(&pFile,cfilename, "rb");
    if (pFile == NULL)
    {
        fputs("File error", stderr);
        return 1;
    }

    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    buffer = (char*)malloc(sizeof(char)*lSize);
	cout << "size :" << lSize << endl;
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        return 2;
    }

    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        return 3;
    }

    fclose(pFile);
    ios::sync_with_stdio(false);
    if (buffer[79]!='\0')//判断格式
    {
        ReadASCII(buffer);
    }
    else
    {
        ReadBinary(buffer);
    }
    ios::sync_with_stdio(true);

    free(buffer);
    return 4;
	*/
}

bool ReadSTLFile::ReadASCII(const char *buffer)
{
    cout<<"ASCII"<<endl;
    unTriangles = 0;
    float x, y, z;
    int i;
    string name, useless;
    stringstream ss(buffer);
    ss >> name>>name ;
    ss.get();
 
    do {
        ss >> useless;
		if (useless.size()<4)
            break;
        getline(ss, useless);
        getline(ss, useless);
        for (i = 0; i < 3; i++)
        {
            ss >> useless >> x >> y >> z;
            pointList.push_back(SPoint3f(x, y, z));
        }
        unTriangles++;
        getline(ss, useless);
        getline(ss, useless);
        getline(ss, useless);
    } while (1);
    return true;
}

bool ReadSTLFile::ReadBinary(const char *buffer)
{
    cout<<"Binary"<<endl;
    const char* p = buffer;
    char name[80];
    int i, j;
    memcpy(name, p, 80);
    p += 80;
    unTriangles= cpyint(p);
    for (i = 0; i < unTriangles; i++)
    {
        p += 12;//跳过头部法向量
        for (j = 0; j < 3; j++)//读取三顶点
            pointList.push_back(SPoint3f(cpyfloat(p), cpyfloat(p), cpyfloat(p)));
        p += 2;//跳过尾部标志
    }
    return true;
}

int ReadSTLFile::NumTri()
{
    return unTriangles;
}

vector<SPoint3f>& ReadSTLFile::PointList()
{
    return pointList;
}

int ReadSTLFile::cpyint(const char*& p)
{
    int cpy;
    memwriter = (char*)&cpy;
    memcpy(memwriter, p, 4);
    p += 4;
    return cpy;
}
float ReadSTLFile::cpyfloat(const char*& p)
{
    float cpy;
    memwriter = (char*)&cpy;
    memcpy(memwriter, p, 4);
        p += 4;
    return cpy;
}
void ReadSTLFile::mshow()
{
    for(int i=0;i<pointList.size();i++)
    {
		cout << pointList[i].x << "," << pointList[i].y << "," << pointList[i].z<<endl;
    }
}
