#include <iostream>
#include <malloc.h>
using namespace std;
void heap (int *arr, int length)
{
    int temp;
    for (int i = (length - 1)/2; i >= 0; i--)
    {
        if (arr[2 * i + 2] >= arr[i] && (2 * i + 2) < length )
        {
        temp = arr[i];
        arr[i] = arr[2 * i + 2];
        arr[2 * i + 2] = temp;
        }
    if (arr[2 * i + 1] >= arr[i] && (2 * i + 1) < length )
    {
        temp = arr[i];
        arr[i] = arr[2 * i + 1];
        arr[2 * i + 1] = temp;
    }

}
}
void sort(int *arr, int length)
{
    int temp;
    heap (arr, length);
    for (int i = length - 1; i >= 0; i--)
    {
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heap (arr, i);
    }
}
int main()
{
    int length;
    int *arr;
    cin >> length;
    arr = (int*)malloc(length * sizeof(int));
    if (arr != NULL)
    {
    for (int index = 0; index < length; index++)
    	cin >> arr[index];

    sort (arr, length);
	for (int index = 0; index < length; index++)
    	cout << arr[index]<< ' ';
    }
    free (arr);
return 0;
}





