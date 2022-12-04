#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <mpi.h>

#define Mx 512
#define Nx 512
#define Ax 2
#define Bx 2

using namespace std;


struct Color
{
	int R;
	int G;
	int B;
};

bool bmpRead(vector<vector<Color>> &imageVec, const char *fileName)
{
	ifstream file(fileName, ios::in | ios::binary);
	if (!file)
		return false;

	// skip header
	const ifstream::off_type headerSize = 54;
	file.seekg(headerSize, ios::beg);
	// read body
	for (size_t y = 0; y != imageVec.size(); ++y)
	{
		for (size_t x = 0; x != imageVec[0].size(); ++x)
		{
			char chR, chG, chB;
			file.get(chB).get(chG).get(chR);

			imageVec[y][x].B = chB;
			imageVec[y][x].G = chG;
			imageVec[y][x].R = chR;

			if (imageVec[y][x].B < 0)
				imageVec[y][x].B = 255 + imageVec[y][x].B;
			if (imageVec[y][x].G < 0)
				imageVec[y][x].G = 255 + imageVec[y][x].G;
			if (imageVec[y][x].R < 0)
				imageVec[y][x].R = 255 + imageVec[y][x].R;
		}
	}

	file.close();

	return true;
}

bool bmpWrite(vector<vector<Color>> &imageVec, const char *fileName)
{
	const int headerSize = 54;

	char header[headerSize] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
		54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0 };

	int ysize = imageVec.size();
	int xsize = imageVec[0].size();

	long file_size = (long)ysize * xsize * 3 + 54;
	header[2] = (unsigned char)(file_size & 0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;

	long width = xsize;
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	long height = ysize;
	header[22] = height & 0x000000ff;
	header[23] = (height >> 8) & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;

	ofstream file(fileName, ios::out | ios::binary);
	if (!file)
		return false;

	// write header
	file.write(header, headerSize);
	// write body
	for (size_t y = 0; y != imageVec.size(); ++y)
	{
		for (size_t x = 0; x != imageVec[0].size(); ++x)
		{
			int chB = (imageVec[y][x].B * 0.114 + imageVec[y][x].G * 0.587 + imageVec[y][x].R * 0.299);

			file.put(chB).put(chB).put(chB);
		}
	}

	file.close();

	return true;
}

int work()
{
	const size_t sizey = 512;
	const size_t sizex = 512;

	vector<vector<Color>> imageVec(sizey, vector<Color>(sizex));
	vector<vector<Color>> imageVecNew(sizey, vector<Color>(sizex));
	vector<vector<int>> codebook(65, vector<int>(Ax * Bx));
	vector<vector<int>> image((Mx / Ax) * (Nx / Bx), vector<int>(Ax * Bx));

	if (!bmpRead(imageVec, "lena.bmp"))
	{
		cout << "Read image error!!" << endl;
		system("PAUSE");
		return -1;
	}

	int count = 0;
	long min = 0;
	long now = 0;
	int temp = 0;
	for (int m = 0; m < Mx / Ax; m++)
	{
		for (int i = 0; i < Nx / Bx; i++)
		{
			temp = 0;
			for (int j = 0; j < Ax; j++)
			{
				for (int k = 0; k < Bx; k++)
				{

					if (count < 64)
						codebook[count][temp] = (imageVec[j + Ax * m][k + Bx * i].B * 0.114 + imageVec[j + Ax * m][k + Bx * i].G * 0.587 + imageVec[j + Ax * m][k + Bx * i].R * 0.299);
					else
					{
						codebook[64][temp] = (imageVec[j + Ax * m][k + Bx * i].B * 0.114 + imageVec[j + Ax * m][k + Bx * i].G * 0.587 + imageVec[j + Ax * m][k + Bx * i].R * 0.299);
					}
					temp++;
					//cout<<temp<<endl;
				}
			}
			if (count > 64)
			{
				int min_index = 0;
				for (int n = 0; n < 64; n++)
				{
					now = 0;
					for (int p = 0; p < Ax * Bx; p++)
					{
						now = now + (codebook[64][p] - codebook[n][p]) * (codebook[64][p] - codebook[n][p]);
					}
					if (n == 0 || now < min)
					{
						min = now;
						min_index = n;
					}
				}
				for (int p = 0; p < Ax * Bx; p++)
				{
					codebook[min_index][p] = (codebook[min_index][p] + codebook[64][p]) / 2;
				}
			}
			count++;
		}
	}
	/***********************************************************************************************************/
	for (int m = 0; m < Mx / Ax; m++)
	{
		for (int i = 0; i < Nx / Bx; i++)
		{
			temp = 0;
			for (int j = 0; j < Ax; j++)
			{
				for (int k = 0; k < Bx; k++)
				{

					codebook[64][temp] = (imageVec[j + Ax * m][k + Bx * i].B * 0.114 + imageVec[j + Ax * m][k + Bx * i].G * 0.587 + imageVec[j + Ax * m][k + Bx * i].R * 0.299);
					temp++;
				}
			}
			if (1)
			{
				int min_index = 0;
				for (int n = 0; n < 64; n++)
				{
					now = 0;
					for (int p = 0; p < Ax * Bx; p++)
					{
						now = now + (codebook[64][p] - codebook[n][p]) * (codebook[64][p] - codebook[n][p]);
					}
					if (n == 0 || now < min)
					{
						min = now;
						min_index = n;
					}
				}
				for (int p = 0; p < Ax * Bx; p++)
				{
					codebook[min_index][p] = (codebook[min_index][p] + codebook[64][p]) / 2;
				}
			}
			count++;
		}
	}

	int count1 = 0;
	int image_index[Mx / Ax][Nx / Bx];
	int n, p, aaa = 0, temp1 = 0;
	for (int m = 0; m < Mx / Ax; m++)
	{
		for (int i = 0; i < Nx / Bx; i++)
		{
			temp1 = 0;
			for (int j = 0; j < Ax; j++)
			{
				for (int k = 0; k < Bx; k++)
				{
					image[count1][temp1] = (imageVec[j + Ax * m][k + Bx * i].B * 0.114 + imageVec[j + Ax * m][k + Bx * i].G * 0.587 + imageVec[j + Ax * m][k + Bx * i].R * 0.299);
					temp1++;
				}
			}
			for (n = 0; n < 64; n++)
			{
				now = 0;
				for (p = 0; p < Ax * Bx; p++)
				{
					now = now + (image[count1][p] - codebook[n][p]) * (image[count1][p] - codebook[n][p]);
				}
				if (n == 0 || now < min)
				{
					min = now;
					aaa = n;
				}
				//

				
			}
			image_index[m][i] = aaa;
			count1++;
		}
	} 
	for (int m = 0; m < Mx / Ax; m++)
	{
		for (int i = 0; i < Nx / Bx; i++)
		{
			temp = 0;
			for (int j = 0; j < Ax; j++)
			{
				for (int k = 0; k < Bx; k++)
				{

					imageVecNew[j + Ax * m][k + Bx * i].R = codebook[image_index[m][i]][temp];
					imageVecNew[j + Ax * m][k + Bx * i].G = codebook[image_index[m][i]][temp];
					imageVecNew[j + Ax * m][k + Bx * i].R = codebook[image_index[m][i]][temp];
					temp++;
					
				}
			}
			count1++;
		}
	}

	//////////////Compression ratio////////////////////
	cout << "Compression Ratio " << ((Mx * Nx) * (log(256) / log(2))) / (((Mx * Nx) / (Ax * Bx)) * (log(64) / log(2))) << endl;
	cout << "Decrease " << (1 - ((((Mx * Nx) / (Ax * Bx)) * (log(64) / log(2))) / ((Mx * Nx) * (log(256) / log(2))))) * 100 << "% Material Quality" << endl;
	cout << ((Mx * Nx) / (Ax * Bx)) * (log(64) / log(2)) << " Material Quality" << endl;

	if (!bmpWrite(imageVec, "clena_clone_cpp.bmp"))
	{
		cout << "Write image error!!" << endl;
		system("PAUSE");
		return -1;
	}

	if (!bmpWrite(imageVecNew, "clena_clone2x2_cpp.bmp"))
	{
		cout << "Write image error!!" << endl;
		system("PAUSE");
		return -1;
	}
	system("PAUSE");
	return 0;
}

// void send_data(char * data, int size)
// {
// 	for( int i = 1 ; i< size ; i++ )
// 	MPI_Send( data, strlen(data)+1, MPI_CHAR, i, 0,MPI_COMM_WORLD);
// }

// void receive_data(int my_rank)
// {
// 	char r_data[500];
// 	// int MPI_Recv(void *buf, int count, MPI_Datatype datatype,int source, int tag, MPI_Comm comm,MPI_Status *status) ;
// 	MPI_Status status;
// 	MPI_Recv(r_data, 500, MPI_CHAR, 0, 0,MPI_COMM_WORLD, &status) ;
// 	// printf("%s %d\n",r_data, my_rank);
// 	cout << r_data << my_rank << endl;
// }


int main(int argc, char **argv){

	MPI_Init(&argc, &argv);

	int world_size, my_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 


	if(my_rank == 0) {
		// receive_data(my_rank);
	}
	if(my_rank == 1){
		work();
		// send_data("Done", world_size);
	}
	else
		int i = 1;
	
	cout << "All Done "<< endl;
	MPI_Finalize();
	return 0;
}
