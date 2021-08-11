#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <cmath>
#include <list>
template <class T>
class astronomer;
using namespace std;
struct points {
	double a;
	double b;
	friend ostream & operator<<(ostream& out, points& p) // overloaded stream insertion operator
	{
		out << '(' << p.a << " , " << p.b << ')';
		return out;
	}
	points& operator=(const points& p) // overloaded assignment operator
	{
		a = p.a;
		b =	p.b;
		return *this;
	}
	points()
	{
		// default constructor
	}
	~points()
	{
		// points destructor
	}
};

struct file {
	double distance;
	int g1;
	int g2;
	bool operator<(const file& f) // overloaded greater than > operator
	{
		if (this->distance < f.distance)
			return true;
		else
			return false;
	}
	bool operator>(const file& f) // overloaded less than < operator
	{
		if (this->distance > f.distance)
			return true;
		else
			return false;
	}
	friend ostream& operator<<(ostream& out, file& f) // overloaded stream insertion
	{
		cout << f.distance << ' ' << f.g1 << ' ' << f.g2 << endl;
		return out;
	}
	file()
	{
		// default constructor
	}
	~file()
	{
		// file destructor
	}
};

template <class T>
T minimum(T a, T b) // template-based minimum calculate function
{
	if (a < b)
		return a;
	else
		return b;
}

template <class T>
void deAllocate(T* arr) // template based deAllocation
{
	delete[] arr;
	arr = NULL;
}

template <class T>
class heap
{
	T* arr;		// array of template object
	int msize;	// maxsize of the heap
	int hsize;	// current size of the heap
public:
	heap() // default constructor
	{
		arr = NULL;
	}
	heap(const int size) // heap paratmerize constructor
	{
		arr = new T[size + 1];
		hsize = 0;
		msize = size + 1;
	}
	~heap() // heap destructor
	{
		delete[] arr;
		arr = NULL;
	}
	void decrease_key(int i, int**& matrix) // function to remove a specific item from the heap
	{
		arr[i].distance = -1000; // reduce that distance to minimal so that it would be at the last
		while (i > 1)
		{
			int r1 = arr[i].g1;
			int c1 = arr[i].g2;
			int r2 = arr[i / 2].g1;
			int c2 = arr[i/ 2].g2;
			swap(matrix[--r1][--c1], matrix[--r2][--c2]); // swap the matrice indicis
			swap(arr[i], arr[i / 2]); // now swap the heap indexes
			i = i / 2; // move towards parent
		}
		// the desired element has now been moved to the top
		remove(matrix); // remove the top element now
	}
	T remove(int**& matrix) // removes a top element from the heap
	{
		T m = arr[1];
		int r1 = arr[1].g1;
		int c1 = arr[1].g2;
		int r2 = arr[hsize].g1;
		int c2 = arr[hsize].g2;
		swap(matrix[--r1][--c1], matrix[--r2][--c2]);
		swap(arr[1], arr[hsize]);
		hsize--;
		heapify(1, hsize, matrix); // call the heapify function
		return m;
	}
	void buildheap(T* a, int n, int**& matrix)
	{
		for (int i = 0; i < n; i++)
		{
			arr[++hsize] = a[i]; // store the whole array initially to the heap. Start storing from index 1
		}
		for (int i = n / 2; i >= 1; i--)
			heapify(i, n, matrix); // as we move to parent call the heapify function
	}
	void heapify(int i, int n, int**& matrix) // heapify function for the heap to retain its order
	{
		int left = 2 * i; // left child position
		int right = 2 * i + 1; // right child position
		int min = 0; // mininum index
		if (left <= n && arr[left] < arr[i])
			min = left;
		else
			min = i;
		if (right <= n && arr[right] < arr[min])
			min = right;
		if (min != i)
		{
			int r1 = arr[i].g1;
			int c1 = arr[i].g2;
			int r2 = arr[min].g1;
			int c2 = arr[min].g2;
			swap(matrix[--r1][--c1], matrix[--r2][--c2]); // swap matrix indicis
			swap(arr[i], arr[min]); // swap both indexes
			heapify(min, n, matrix); // recursively call the heapify function 
		}
	}
	void print() // print heap
	{
		cout << endl << ' ';
		for (int i = 1; i <= hsize; i++)
		{
			cout << i << '\t' << arr[i] << ' ';
		}
		cout << endl;
	}
	friend class astronomer<T>;
};
template <class T>
class astronomer
{
	vector<list<points>> vl1;
	heap<T> h;			// heap to calculate minimum
	int** matrix;		// matrix to store the heap index
	int tgbd;			// the groups to be divided
	int curr_grps;		// how many groups currently this class has
	int total_groups;	// total groups that it initially has
public:
	astronomer()
	{
		matrix = NULL;
	}
	astronomer(const int length, const int groups) : h(length * (length - 1) / 2) // paratemerized constructor of heap's class
	{
		matrix = new int*[length];
		for (int i = 0; i < length; i++) // make n*n matrix
		{
			matrix[i] = new int[length];
		}
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < length; j++)
			{
				matrix[i][j] = 0;	// initially all matrix indexes will contain zero
			}
		}
		// initialize total groups and the groups to be divided inputs 
		total_groups = length;
		curr_grps = total_groups;
		tgbd = groups;
	}
	double calculate_distance(double a, double b, double c, double d) // this function takes 4 points and calculates the distance between them
	{
		double a1 = pow((a - c), 2); // (x1 - x2) ^ 2
		double a2 = pow((b - d), 2); // (y1 - y2) ^ 2
		return pow((a1 + a2), 0.50000); // ((x1 - x2) ^ 2 + (y1 - y2) ^ 2) ^ 1/2
	}
	void mer_row(int row, int col) // this function merges row
	{
		int a1 = 0, a2 = 0;
		file f;
		for (int i = 0; i < row - 1; i++) 
		{
			// get the indexes of from the matrix
			a1 = matrix[i][row - 1];
			a2 = matrix[i][col - 1];
			f = minimum<file>(h.arr[a1], h.arr[a2]); // find the minimum between the two indexes of heap
			if (f.distance != -1000) // check whether that element is already deleted
			{
				h.arr[a1] = f;
				h.arr[a1].g1 = i + 1;	// assign the row
				h.arr[a1].g2 = row;		// assign the column
				h.decrease_key(a2, matrix); // remove that element from heap
			}
		}
		for (int i = row; i < col - 1; i++)
		{
			// get the indexes of from the matrix
			a1 = matrix[row - 1][i];
			a2 = matrix[i][col - 1];
			f = minimum<file>(h.arr[a1], h.arr[a2]); // find the minimum between the two indexes of heap
			if (f.distance != -1000) // check whether that element is already deleted
			{
				h.arr[a1] = f;
				h.arr[a1].g1 = row;		// assign the row
				h.arr[a1].g2 = i + 1;	// assign the column
				h.decrease_key(a2, matrix); // remove that element from heap
			}
		}
	}
	void mer_col(int row, int col) // this function merges column
	{
		int a1 = 0, a2 = 0;
		file f;
		for (int i = col; i < total_groups; i++) // for column
		{	
			// get the indexes of from the matrix
			a1 = matrix[row - 1][i];
			a2 = matrix[col - 1][i];
			f = minimum<file>(h.arr[a1], h.arr[a2]); // find the minimum between the two indexes of heap
			if (f.distance != -1000) // check whether that element is already deleted
			{ 
				h.arr[a1] = f;
				h.arr[a1].g2 = i + 1;	// assign the column
				h.arr[a1].g1 = row;		// assign the row
				h.decrease_key(a2, matrix); // remove that element from heap
			}
		}
	}
	void merge_groups() // this function merges groups that have minimum distance i.e are top of heap
	{
		if (tgbd > 0)
		{
			while (curr_grps > tgbd) // when current groups becomes equal to the divided groups then exit from the loop
			{
				file f = h.remove(matrix); // remove the top element from the heap and update the matrix
				/*for (int i = 1; i <= (total_groups * (total_groups - 1) / 2); i++)
				{
					int row = h.arr[i].g1;
					int col = h.arr[i].g2;
					matrix[--row][--col] = i;
				}*/
				int row = f.g1;
				int col = f.g2;
				for (auto j = vl1[col - 1].begin(); j != vl1[col - 1].end(); j++)
				{
					points a = *j;
					vl1[row - 1].push_back(a); // merge that group
				}
				vl1[col - 1].clear(); // the group's been merged so clear that list (i.e make it NULL)
				mer_row(row, col); // function for row
				mer_col(row, col); // function for col
				--curr_grps; // reduce the current groups as a group is merged now
			//	print();
			}
		}
		else
			cout << "\n\tGroups to be divided-input must be > 0\n";
	}
	void file_read(ifstream &fin, file *&f, int length) // this function reads data from the file
	{
		points *arr = new points[length];
		for (int i = 0; i < length; i++) // reading the points in the file and pushing them into points array and vector of linkedlist
		{
			fin >> arr[i].a;
			fin >> arr[i].b;
			list<points> l2;
			l2.push_back(arr[i]);
			vl1.push_back(l2);
		}
		f = new file[length * (length - 1) / 2]; // make an array of (N * (N - 1) / 2) to store distances between points
		int k = 0;
		for (int i = 0; i < length; i++)
		{
			for (int j = i + 1; j < length; j++)
			{
				f[k].distance = calculate_distance(arr[i].a, arr[i].b, arr[j].a, arr[j].b);
				f[k].g1 = i + 1;
				f[k].g2 = j + 1;
				k++;
			}
		}
		h.buildheap(f, length * (length - 1) / 2, matrix); // call the build heap function on the array
		for (int i = 1; i <= (length * (length - 1) / 2); i++) // this loop initialize the matrix of indexes
		{
			int row = h.arr[i].g1;
			int col = h.arr[i].g2;
			matrix[--row][--col] = i;
		}
		// deAllocate all the dynamic memory taken
		deAllocate<file>(f);
		deAllocate<points>(arr);
		fin.close(); // close the file
	}
	void print()
	{
		h.print(); // print the heap
		//for (int i = 0; i < total_groups; i++) // print index matrix
		//{
		//	for (int j = 0; j < total_groups; j++)
		//	{
		//		cout << matrix[i][j] << ' ';
		//	}
		//	cout << endl;
		//}	
		cout << endl << "\tGroups are:- \n";
		for (int i = 0; i < total_groups; i++) // now printing the vector of linked list
		{
			cout << i + 1 << ": ";
			for (auto j = vl1[i].begin(); j != vl1[i].end(); j++)
				cout << *j << '\t' ;
			cout << endl;
		}
	}
	~astronomer() // destructor
	{
		for (int i = 0; i < total_groups; i++)
		{
			delete[] matrix[i];
		}
		delete[] matrix;
		matrix = NULL;
	}
};

void main()
{
	int a = 0, length = 0;
	ifstream fin("L18_1247.txt");
	fin >> length;
	file *f1;
	char f;
	fin >> f;
	fin >> a;
	astronomer<file> as(length, a); // create the astronomer class with required information
	as.file_read(fin, f1, length); // read information from the file
	as.print();
	as.merge_groups(); // call the merge_groups function
	as.print();
	system("pause");
}
