#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
	
	

	for (int reg = 0; reg < 8; reg++) {
		int modrm = 0xC0;

		modrm += (reg<<3);

		for (int rm = 0; rm < 8; rm++) {
			modrm &= 0xF8;

			modrm += rm;


			cout << hex << setfill('0') << setw(2) << modrm << endl;
			
		}
	}

	

	return 0;
}