#include <fstream>
#include <vector>
using namespace std;
ifstream ifs("in.txt", ios::in);
ofstream ofs("out.txt", ios::out);
string a;
bool allowoutput = false;

int main()
{
	ifstream ifs("C:/Users/LENOVO T540P/Desktop/MarioMap1-1.txt", ios::in);
	ofstream ofs("C:/Users/LENOVO T540P/Desktop/MarioMap1.txt", ios::out);
	string a;
	bool allowoutput = false;
	int count = 0;
	while (!ifs.eof())
	{
		
		ifs >> a;
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] == '"')
				if (!allowoutput)
				{
					allowoutput = true;
					continue;
				}
				else {
					count++;
					if (count == 176)
					{
						ofs << endl;
						count = 0;
					}
					else
						ofs << " ";
					allowoutput = false;
				}
			if (allowoutput)
			{
				ofs << a[i];
			}
		}
	}

	ifs.close();
	system("pause");
}