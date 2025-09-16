#include <bits/stdc++.h>
using namespace std;

// Hàm trợ giúp để kiểm tra điều kiện và báo lỗi nếu sai
void ensure(bool condition, const string& message) {
    if (!condition) {
        cerr << "Validation Error: " << message << endl;
        exit(1); // Thoát với mã lỗi khác 0
    }
}

int main() {
    // Tăng tốc độ đọc nếu cần
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Nếu mọi thứ hợp lệ, trả về 0
    return 0;
}
