#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream inp (argv[1]);
    ifstream ans (argv[2]);

    int cnt = 0, tmp;
    while (ans >> tmp) cnt ++;

    cout << "cnt = " << cnt;
    return 0;
}
