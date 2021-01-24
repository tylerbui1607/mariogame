#include <fstream>
#include <vector>
#include "ArrangeGrid.h"
using namespace std;


int main()
{
	int left, top, right, bottom, x, y, type, cellwidth, cellheight;
	int bboxwidth; int bboxheight;
	ifstream ifs("LoadGrid1_4.txt", ios::in);
	ofstream ofs("gridinfo.txt", ios::out);
	string a;
	bool allowoutput = false;
	int count = 0;
	ifs >> cellwidth >> cellheight;
	while (!ifs.eof())
	{

		ifs >> type;
		ifs >> x >> y;
		ifs >> bboxwidth >> bboxheight;
		ofs << type << " " << (int)(x * 1.0 / cellwidth) << " " << (int)(y * 1.0 / cellheight) << " " << (int)((x + bboxwidth) * 1.0 / cellwidth) << " " << (int)((y + bboxheight) * 1.0 / cellheight) << endl;

	}

	ifs.close();

	system("pause");
}