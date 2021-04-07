#include<bits/stdc++.h>
using namespace std;

vector<string> in;
void read_input() {
    for (int i = 0; i < 10; i++) {
        ifstream in_file;
        stringstream ss;
        in_file.open("sample." + to_string(i + 1));
        ss << in_file.rdbuf();
        in.push_back(ss.str());
    }
}

class String_Hashing {
    int MOD = 1e9 + 7, n;
    vector<pair<int, int>> coefficients;
    pair<int, int> seeds = {13, 17};
    vector<vector<pair<int, int>>> hashed_strings;

    void compute_coefficients() {
        coefficients.assign(n, {1, 1});
        for (int i = 1; i < n; i++) {
            coefficients[i].first = (1LL * coefficients[i - 1].first * seeds.first) % MOD;
            coefficients[i].second = (1LL * coefficients[i - 1].second * seeds.second) % MOD;
        }
    }

public:

     explicit String_Hashing(int n) {
        this->n = n;
        compute_coefficients();
    }
    void hash_string(const string &s) {
        hashed_strings.emplace_back(vector<pair<int, int>>(s.size() + 1, {0, 0}));
        auto &cur = hashed_strings.back();

        for (int i = 1; i <= s.size(); i++) {
            cur[i].first = (cur[i - 1].first + (1LL * coefficients[i - 1].first * s[i - 1]) % MOD) % MOD;
            cur[i].second = (cur[i - 1].second + (1LL * coefficients[i - 1].second * s[i - 1]) % MOD) % MOD;
        }


    }

    pair<int, int> query(int string_index, int start_index, int end_index) {
        pair<int, int> result;
        end_index++;
        result.first =
                ((hashed_strings[string_index][end_index].first - hashed_strings[string_index][start_index].first) +
                 MOD) % MOD;
        result.first = (1LL * result.first * coefficients[n - 1 - start_index].first) % MOD;

        result.second =
                ((hashed_strings[string_index][end_index].second - hashed_strings[string_index][start_index].second) +
                 MOD) % MOD;
        result.second = (1LL * result.second * coefficients[n - 1 - start_index].second) % MOD;

        return result;
    }


};

vector<pair<int, int>> p(int m, String_Hashing &sh) {

    map<pair<int, int>, vector<pair<int, int>>> mp;
    for (int i = 0; i + m - 1 < in[0].size(); i++) {
        int end = i + m - 1;
        pair<int, int> q_value = sh.query(0, i, end);
        mp[q_value].push_back({0, i});
    }

    pair<int, int> v = {-1, -1};
    for (int string_index = 1; string_index < in.size(); string_index++) {
        for (int i = 0; i + m - 1 < in[string_index].size(); i++) {
            int end = i + m - 1;
            pair<int, int> q_value = sh.query(string_index, i, end);
            auto it = mp.find(q_value);
            if (it != mp.end() && it->second[0].first != string_index)
                v = q_value;
            mp[q_value].push_back({string_index, i});
        }
    }

    if (v.first == -1) {
        return vector<pair<int, int>>{{-1, -1}};
    } else {
        return mp[v];
    }

}


int main() {

    /* Time Complexity: O(nlog^2(n))
     * Space Complexity: O(n)
     * Algorithms used: Rabin-Karp string hashing, binary search
     * Datastructures used: map
     * */

    read_input();

    int maxi = 0;
    for (auto &s:in)
        maxi = max(maxi, (int) s.size());


    String_Hashing sh(maxi);
    for (auto &s:in)
        sh.hash_string(s);


    vector<pair<int, int>> ans = {{0, 0}};

    int l = 0, r = maxi;
    while (l < r) {
        int m = l + (r - l + 1) / 2;
        vector<pair<int, int>> result = p(m, sh);
        if (result[0].first != -1) {
            l = m;
            ans = result;
        } else r = m - 1;
    }

    cout << "length = " << l << '\n';
    for (auto &x:ans) {
        cout << "File Name: " << "sample." + to_string(x.first + 1) << '\n';
        cout << "Offset: " << x.second << '\n';
    }


    // check
    /*
        pair<int, int> p = sh.query(ans[0].first, ans[0].second, ans[0].second + l - 1);
        for (int string_index = 0; string_index < in.size(); string_index++) {
            for (int i = 0; i + l - 1 < in[string_index].size(); i++) {
                pair<int, int> q = sh.query(string_index, i, i + l - 1);
                if (p.first == q.first && p.second == q.second) {
                    cout << string_index << '\n';
                }
            }
        }
        bool x = in[ans[0].first].substr(ans[0].second, l) == in[ans[1].first].substr(ans[1].second, l);
        cout << (x ? "equal" : "not equal") << '\n';
     */

    return 0;
}