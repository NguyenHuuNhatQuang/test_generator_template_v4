#include <bits/stdc++.h>
using namespace std;

#ifdef GENERATOR
string subtask = "";

int regis_subtask (string s) // subtask registration
{
    subtask = s;
    return 0;
}
#endif // GENERATOR

namespace Templates
{
    mt19937_64 mt(28042007);

    long long Rand (long long l, long long h)
    {
        if (h < l)
        {
            cerr << "Loi khi goi ham Rand (" << l << ", " << h << ").";
            exit (-1);
        }

        return mt () % (h - l + 1) + l;
    }

    long long log_rand (long long t) // sinh mot so ngau nhien tu 1 den t
    {
        if (t <= 0)
        {
            cerr << "Loi khi goi ham log_rand (" << t << ").";
            exit (-1);
        }

        int x = 64 - __builtin_clzll(t);
        return Rand(1, min(t, (1LL << Rand(1, x))));
    }

    bool bit01 () { // tra ve 0 hoac 1
        return rand () % 2;
    }
    char az () { // tra ve ki tu ngau nhien tong ['a' .. 'z']
        return (char)(Rand('a', 'z'));
    }
    char AZ () { // tra ve ki tu ngau nhien tong ['A' .. 'Z']
        return (char) (Rand('A', 'Z'));
    }
    char num () { // tra ve ['0' .. '9']
        return '0' + Rand(0, 9);
    }

    template <class T> void Shuffle (vector <T> &arr) // hoan vi mot mang
    {
        int n = arr.size ();

        for (int i = 0; i < n; i ++)
            swap (arr[i], arr[Rand (0, i)]);
    }

    string bignum(long long l) // sinh mot so lon gom l chu so
    {
        assert (l > 0);
        if (l <= 0)
        {
            cerr << "Loi khi goi ham bignum (" << l << ").";
            exit (-1);
        }

        string s = " ";
        s[0] = char (Rand (1, 9) + '0');
        for(int i = 1; i <= l; i ++)
        {
            s += " ";
            s[s.size() - 1] = num ();
        }
        return s;
    }

    vector <pair <int, int>> tree (int ver, int d) // tra ve mot cay
    {
        if (ver <= 0)
        {
            cerr << "Loi khi goi ham tree (" << ver << ", " << d << ").";
            exit (-1);
        }

        if (d <= 0) d = ver;

        vector <int> perm;
        for (int i = 1; i <= ver; i ++) perm.push_back (i);
        Shuffle (perm);

        vector <pair <int, int>> ans;
        for (int i = 2; i <= ver; i ++)
        {
            int par = Rand(max (1, i - d), i - 1);
            ans.push_back (make_pair (perm[par - 1], perm[i - 1]));
        }

        Shuffle (ans);
        return ans;
    }

    string bracket (int n, int d) // tra ve mot day ngoac dung do dai 2 * n
    {
        if (n <= 0)
        {
            cerr << "Loi khi goi ham bracket (" << n << ", " << d << ").";
            exit (-1);
        }

        vector <pair <int, int>> edges = tree (n + 1, d);
        vector <int> adj[n + 2], st;
        string ans;

        for (auto i: edges)
        {
            adj[i.first].push_back (i.second);
            adj[i.second].push_back (i.first);
        }

        st.push_back (0);
        st.push_back (1);

        while (int (st.size ()) > 1)
        {
            int t = st.back ();
            if (adj[t].size ())
            {
                if (adj[t].back () != st[st.size () - 2])
                {
                    ans += '(';
                    st.push_back (adj[t].back ());
                }
                adj[t].pop_back ();
            }
            else
            {
                ans += ')';
                st.pop_back ();
            }
        }

        ans.pop_back ();
        return ans;
    }

    vector<pair<int, int>> connected_graph (int ver, int edge) // tra ve cac canh cua mot don do thi lien thong
    {
        if (!(ver > 0 && edge >= ver - 1 && edge <= ver * (ver - 1) / 2))
        {
            cerr << "Loi khi goi ham connected_graph (" << ver << ", " << edge << ").";
            exit (-1);
        }

        unordered_map<long long, bool> used;
        used[0] = true;

        vector<pair<int, int>> ans = tree (ver, 0);

        for (auto i: ans)
        {
            if (i.first < i.second) swap (i.first, i.second);
            used[1LL * i.first * ver + i.second] = true;
        }

        for(int i = ver; i <= edge; i ++)
        {
            int a = 0, b = 0;
            while(a <= b or used[1LL * a * ver + b])
            {
                a = Rand(1, ver);
                b = Rand(1, ver);
            }
            used[1LL * a * ver + b] = true;
            if(bit01 ()) swap(a, b);
            ans.push_back(make_pair(a, b));
        }

        return ans;
    }

    vector<pair<int, int>> graph(int ver, int edge) // tra ve cac canh cua mot don do thi ngau nhien
    {
        if (!(ver > 0 && edge >= 1 && edge <= ver * (ver - 1) / 2))
        {
            cerr << "Loi khi goi ham graph (" << ver << ", " << edge << ").";
            exit (-1);
        }

        unordered_map<long long, bool> used;
        used[0] = true;

        vector<pair<int, int>> ans;
        for(int i = 1; i <= edge; i ++)
        {
            int a = 0, b = 0;
            while(a <= b or used[1LL * a * ver + b])
            {
                a = Rand(1, ver);
                b = Rand(1, ver);
            }
            used[1LL * a * ver + b] = true;
            if(bit01 ()) swap(a, b);
            ans.push_back(make_pair(a, b));
        }

        return ans;
    }
}
