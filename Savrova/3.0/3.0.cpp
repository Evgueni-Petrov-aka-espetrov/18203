#include <string>
#include <iostream>
using namespace std;
int sort (int *arr, int length)
{
    int temp, index1, index2;
    for (index1 = 0; index1 < length ; index1++)
    	for ( index2 = 0; index2 < length - index1 - 1; index2++)
		if (arr[index2] > arr[index2 + 1])
		{
		    temp = arr[index2];
		    arr[index2] = arr[index2 + 1];
		    arr[index2 +1] = temp;
		}
return *arr;
}
int main()
{
    int length;
    int *arr;
    cin >> length;
    arr = (int*)malloc(length * sizeof(int));
    for (int index = 0; index < length; index++)
    	cin >> arr[index];
    sort (arr, length);
    for (int index = 0; index < length; index++)
    	cout << arr[index]<< ' ';
    return 0;
}
