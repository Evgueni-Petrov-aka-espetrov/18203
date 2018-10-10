#include <string>
#include <iostream>
using namespace std;
char NumToChar(int n)
{
    if ((n >= 0) && (n <= 9))
        return '0' + n;
    else
        return 'A' - 10 + n;
}
int CharToNum(char a)
{
    if ((a >= '0') && (a <= '9'))
        return a - '0';
    else if ((a >= 'a') && (a <= 'f'))
        return a - 'a' + 10;
    else if ((a >= 'A') && (a <= 'F'))
        return a - 'A' + 10;
}
string ConvertIntPart(string digit, int radix1, int radix2)
{
    long long n = 0;
    for (int i = 0; i < digit.length(); i++)
    {
            n = n * radix1 + CharToNum(digit[i]);
    }
    string result = "";
    if (n == 0)
        result = '0';
    else
    while (n!= 0)
    {
        result = NumToChar(n % radix2) + result;
        n /= radix2;
    }
    return result;
}
string ConvertFrancPart (string digit, int radix1, int radix2)
{
   int digitIndex;
   double fracPart = 0, pow = radix1;
   char ans[12];
   for (int digitIndex = 0; digitIndex < digit.length(); digitIndex++)
   {
       fracPart += CharToNum(digit[digitIndex]) / pow;
       pow*=radix1;
   }
   for (int digitIndex = 0; digitIndex < 12; digitIndex++)
    {
      fracPart*=radix2;
      ans[digitIndex] = NumToChar((int)fracPart);
      fracPart-=(int)fracPart;
    }
    return ans;
}
int check ( int radix1, int radix2, string digit)
{
    int dot(0);
    if (radix1 < 2 || radix1 > 16 || radix2 < 2 || radix2 > 16)
        return 1;
    else
    {
        for (int i = 0; i < digit.length(); i++)
        {
            if (CharToNum(digit[i]) >= radix1 && digit[i] != '.')
                return 1;
            else if (digit[i] == '.')
            {
                if (dot != 0 || i == 0 || digit[i + 1] == '\0')
                    return 1;
                else
                    dot++;
            }
        }
    }
}
int main()
{
    string digit, digitInt, digitFranc;
    int radix1, radix2, dot;
    cin >> radix1 >> radix2 >> digit;
    if (check ( radix1, radix2, digit) == 1)
        cout << "bad input";
    else
    {
        dot = digit.find('.');
        if (dot != -1)
        {
            digitFranc = digit.substr(dot + 1);
            digitInt = digit.substr(0, dot );
            cout << ConvertIntPart(digitInt, radix1, radix2) << '.' << ConvertFrancPart(digitFranc, radix1, radix2);
        }
        else
            cout << ConvertIntPart(digit, radix1, radix2);
    }
    return 0;
}

