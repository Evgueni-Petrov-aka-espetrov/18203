#include <string>
#include <iostream>
using namespace std;

void quicksort (int *arr, int first, int last)
{
	int middle, temp;
	int left = first, right = last;
	middle = arr[(left + right) / 2];

	while (left <= right)
	{
		while (arr[left] < middle)
			left ++;
		while (arr[right] > middle)
			right --;
		if (left <= right)
		{
			temp = arr[left];
			arr[left] = arr[right];
			arr[right] = temp;
			left ++;
			right --;
		}
	};
	if (first  < right)
		quicksort (arr, first, right);
	if (left < last )
		quicksort (arr, left,last);
}

int main()
{
    int length;
    int *arr;
    cin >> length;
    arr = (int*)malloc(length * sizeof(int));
    for (int index = 0; index < length; index++)
   	 cin >> arr[index];
    quicksort (arr, 0, length - 1);
    for (int index = 0; index < length; index++)
    	cout << arr[index]<< ' ';

return 0;
}
