#include <iostream>
#include <string>
#include <vector>

#define GENERATOR
#include "generator.cpp"
#include "create.h" // file này giờ đây chứa gần như toàn bộ logic

using namespace std;

int main() {
    // 1. Lấy cấu hình từ người dùng hoặc file
    Create::GeneratorConfig config = Create::load_or_create_config();

    // 2. Chuẩn bị môi trường và biên dịch
    using namespace Create;
    create_root(config.prob_name, config.overwrite_choice);
    compile_solution();
    compile_validator();
    compile_checker();

    // 3. Mở file log
    string log_filename = config.prob_name + "_log.csv";
    ofstream log_file(log_filename);
    if (!log_file.is_open()) {
        cerr << "LOI: Khong the mo file log " << log_filename << " de ghi." << endl;
        return 1;
    }
    log_file << "Test Order,Input Gen Time (ms),Input Valid?,Validator Time (ms),Solution Error?,Solution Time (ms),Checker Time (ms),Checker Feedback\n";

    cerr << "Bat dau sinh test\n----------\n";

    // 4. Chạy vòng lặp sinh test
    bool generation_ok = true;
    for (int i = 1; i <= config.testnum; i++) {
        if (!run_test_case(i, config.prob_name, log_file)) {
            generation_ok = false;
            break;
        }
    }

    // 5. Đóng file và báo cáo kết quả
    log_file.close();
    cerr << "--------------------\n";
    if (generation_ok) {
        cerr << "Da sinh thanh cong " << config.testnum << " test.\n";
    } else {
        cerr << "Qua trinh sinh test bi dung do co loi.\n";
    }
    cerr << "Log chi tiet da duoc ghi vao file: " << log_filename << endl;
    cerr << "Ket thuc chuong trinh sinh test.\n";

    return 0;
}
