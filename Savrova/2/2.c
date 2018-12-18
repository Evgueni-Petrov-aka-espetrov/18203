#include <iostream>
#define MAX_LENGTH 11
using namespace std;

void swap(char* arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int permutation(char* arr, int amount)
{
    int index1 = amount - 1;
    while (index1 > 0 && arr[index1 - 1] >= arr[index1])
        index1--;
    if (!index1)
        return 0;
    int index2 = amount - 1;
    while (arr[index2] <= arr[index1 - 1])
        index2--;
    swap(arr, index1 - 1, index2);
    index2 = amount - 1;
    while (index1 ++ < index2 --)
        swap(arr, index1, index2);
    return 1;
}

int check(char* arr)
{
    int s[MAX_LENGTH - 1] = { 0 };
    for (int index  = 0; index < strlen(arr); index++)
     {
        if(s[arr[index] - '0'] || arr[index] > '9' || arr[index] < '0')
            return 0;
        s[arr[index] - '0'] = 1;
    }
    return 1;
}

int main()
{
    int amount;
    char arr[MAX_LENGTH];
    cin >> arr >> amount;
    if (!check(arr))
        cout << "bad input";
    else
        while (permutation(arr, strlen(arr)) && amount--)
        cout << arr << endl;
    return 0;
}
