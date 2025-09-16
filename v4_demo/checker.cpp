#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream inp (argv[1]);
    ifstream ans (argv[2]);

    int a, b, sum;
    inp >> a >> b;
    ans >> sum;

    if (sum != a + b) cout << "Output isn't correct!";
    else cout << "Output is correct!";
}
