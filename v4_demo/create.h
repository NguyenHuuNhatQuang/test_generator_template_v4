#include<bits/stdc++.h>
using namespace std;

namespace Create
{
    // Cấu trúc để lưu trữ toàn bộ cài đặt
    struct GeneratorConfig {
        string prob_name;
        int testnum;
        char overwrite_choice;
        bool loaded_from_file = false;
    };

    string format_time(double time_ms) {
        stringstream ss;
        ss << fixed << setprecision(2) << time_ms;
        return ss.str();
    }

    // <<< HÀM MỚI: LƯU CẤU HÌNH RA FILE >>>
    void save_config(const GeneratorConfig& config) {
        const string CONFIG_FILE = "generator_config.txt";
        ofstream saveConfigFile(CONFIG_FILE);
        if (saveConfigFile.is_open()) {
            saveConfigFile << config.prob_name << endl;
            saveConfigFile << config.testnum << endl;
            saveConfigFile << config.overwrite_choice << endl;
            saveConfigFile.close();
        } else {
            cerr << "CANH BAO: Khong the luu file cau hinh '" << CONFIG_FILE << "'." << endl;
        }
    }

    // <<< HÀM MỚI: ĐỌC VÀ LẤY CẤU HÌNH TỪ NGƯỜI DÙNG >>>
    GeneratorConfig load_or_create_config() {
        GeneratorConfig config;
        const string CONFIG_FILE = "generator_config.txt";
        ifstream configFile(CONFIG_FILE);

        if (configFile.is_open()) {
            string loaded_prob_name;
            int loaded_testnum;
            char loaded_overwrite_choice;

            if (getline(configFile, loaded_prob_name) && (configFile >> loaded_testnum) && (configFile >> loaded_overwrite_choice)) {
                cout << "Da tim thay cai dat cu:" << endl;
                cout << "  - Ten bai: \"" << loaded_prob_name << "\"" << endl;
                cout << "  - So test: " << loaded_testnum << endl;
                cout << "  - Hanh dong voi test cu: " << (loaded_overwrite_choice == 'y' ? "Xoa" : "Luu vao history") << endl;
                cout << "Ban co muon su dung lai khong? (y/n): ";
                char choice;
                cin >> choice;
                while (tolower(choice) != 'y' && tolower(choice) != 'n') {
                    cout << "Nhap lai (y/n): "; cin >> choice;
                }

                if (tolower(choice) == 'y') {
                    config.prob_name = loaded_prob_name;
                    config.testnum = loaded_testnum;
                    config.overwrite_choice = loaded_overwrite_choice;
                    config.loaded_from_file = true;
                    cout << "=> Da ap dung cai dat cu." << endl;
                }
            }
            configFile.close();
        }

        if (!config.loaded_from_file) {
            cout << "----------------------------------" << endl;
            cout << "Nhap so luong test: ";
            cin >> config.testnum;
            while(cin.fail()) {
                cout << "Vui long nhap mot con so. Nhap lai: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> config.testnum;
            }

            cout << "Nhap ma bai: ";
            cin >> config.prob_name;

            cout << "Khi thu muc test da ton tai, ban muon Xoa (y) hay Luu vao history (n)? [y/n]: ";
            cin >> config.overwrite_choice;
            while (tolower(config.overwrite_choice) != 'y' && tolower(config.overwrite_choice) != 'n') {
                cout << "Nhap lai (y/n): ";
                cin >> config.overwrite_choice;
            }
            cout << "----------------------------------" << endl;

            // Lưu lại cấu hình mới nhập
            save_config(config);
        }

        return config;
    }

    string str (long long a)
    {
        if(a == 0) return "0";
        if(a < 0) return "-" + str(-a);
        string ans = "";
        for(int i = 1; i <= 4; i ++)
        {
            ans = "0" + ans;
            ans[0] = (a % 10) + '0';
            a /= 10;
        }
        return ans;
    }

    // Trong file create.h

    void copy_test (string prob_name, string subtask, int iTest, bool silent_mode = false)
    {
        if (!silent_mode) cerr << "Dang tao thu muc chua test #" << iTest << ".\n";

        // Sử dụng to_string chuẩn thay vì str để không zero-padding
        stringstream ss;
        ss << setw(4) << setfill('0') << iTest; // Vẫn giữ zero-padding 4 chữ số cho thư mục test con
        string test_folder_index = ss.str();

        string folder_link = prob_name + "\\" + test_folder_index + (subtask.empty() ? "" : ("_" + subtask)); // Thêm subtask nếu có
        string check_cmd = "if exist \"" + folder_link + "\\\" (exit /b 0) else (exit /b 1)"; // Sửa logic: trả về 0 nếu TỒN TẠI

        // Xóa thư mục con nếu nó đã tồn tại để ghi đè
        if (system(check_cmd.c_str()) == 0) {
             if (!silent_mode) cerr << "Thu muc test con da ton tai, xoa bo: " << folder_link << endl;
             system(("rmdir /s /q \"" + folder_link + "\"").c_str());
        }

        system(("mkdir \"" + folder_link + "\"").c_str());

        if (!silent_mode) cerr << "Dang sao chep cac file input va output.\n";

        string inp_file = prob_name + ".inp";
        string out_file = prob_name + ".out";

        string copy_inp_cmd = "copy \"" + inp_file + "\" \"" + folder_link + "\" > nul"; // Ẩn output của copy
        string copy_out_cmd = "copy \"" + out_file + "\" \"" + folder_link + "\" > nul"; // Ẩn output của copy

        // Giữ lại các cảnh báo quan trọng này
        if (system(copy_inp_cmd.c_str()) != 0) {
             cerr << "CANH BAO: Khong the sao chep file " << inp_file << " cho test #" << iTest << endl;
        }
        if (system(copy_out_cmd.c_str()) != 0) {
            cerr << "CANH BAO: Khong the sao chep file " << out_file << " cho test #" << iTest << endl;
        }

        if (!silent_mode) cerr << "Da sao chep xong.\n";
    }

    // Trong file create.h

    // Thêm một tham số overwrite_choice với giá trị mặc định là '\0' (null)
    void create_root (string prob_name, char overwrite_choice = '\0')
    {
        string check_cmd = "if exist \"" + prob_name + "\\\" (exit /b 0) else (exit /b 1)";
        bool existed = (system (check_cmd.c_str()) == 0);

        if (existed)
        {
            cerr << "Thu muc \"" << prob_name << "\" da ton tai.\n";

            string _ok;
            // <<< BẮT ĐẦU THAY ĐỔI >>>
            // Chỉ hỏi người dùng nếu không có lựa chọn nào được truyền vào
            if (overwrite_choice == '\0') {
                cerr << "Xoa bo test cu (y) hay luu vao history (n)? [y/n]: ";
                cin >> _ok;
                while (_ok != "y" && _ok != "n")
                {
                    cerr << "Nhap lai (y/n): ";
                    cin >> _ok;
                }
            } else {
                // Sử dụng lựa chọn đã được truyền vào từ file config
                _ok = overwrite_choice;
                cerr << "Thuc hien hanh dong da luu: "
                     << (_ok == "y" ? "Xoa bo test cu." : "Luu vao history.") << endl;
            }
            // <<< KẾT THÚC THAY ĐỔI >>>

            if (_ok == "y")
            {
                cerr << "Dang xoa bo test cu...\n";
                string delete_cmd = "rmdir /s /q \"" + prob_name + "\"";
                if (system(delete_cmd.c_str()) != 0) {
                     cerr << "CANH BAO: Khong the xoa thu muc " << prob_name << endl;
                } else {
                    cerr << "Da xoa bo test cu.\n";
                }
                existed = false;
            }
            else
            {
                cerr << "Dang di chuyen bo test cu vao history...\n";
                string history_root = prob_name + "_history";
                string mkdir_history_cmd = "mkdir \"" + history_root + "\" 2> nul";
                system(mkdir_history_cmd.c_str());

                int history_index = 1;
                string current_history_check_path;
                string check_history_item_cmd;
                do {
                    current_history_check_path = history_root + "\\" + prob_name + "_" + to_string(history_index);
                    check_history_item_cmd = "if exist \"" + current_history_check_path + "\\\" (exit /b 0) else (exit /b 1)";
                    if (system(check_history_item_cmd.c_str()) == 0) {
                        history_index++;
                    } else {
                        break;
                    }
                } while (true);

                string target_history_path = current_history_check_path;
                string move_cmd = "move \"" + prob_name + "\" \"" + target_history_path + "\"";

                if (system(move_cmd.c_str()) != 0) {
                     cerr << "LOI: Khong thể di chuyển \"" << prob_name << "\" vào \"" << target_history_path << "\". Giu nguyen thu muc cu va thoat." << endl;
                     exit(1);
                } else {
                     cerr << "Da di chuyen bo test cu vao: \"" << target_history_path << "\"\n";
                     existed = false;
                }
            }
        }

        if (!existed) {
             string mkdir_cmd = "mkdir \"" + prob_name + "\"";
             if(system(mkdir_cmd.c_str()) != 0) {
                 cerr << "LOI: Khong thể tạo thư mục gốc mới \"" << prob_name << "\". Vui lòng kiểm tra lại." << endl;
                 exit(1);
             }
        }
    }

    void compile_solution ()
    {
        cerr << "Dang bien dich file loi giai (solution.cpp).\n";
        // Nên dùng c++17 trở lên và bật cảnh báo
        int result = system("g++ -std=c++14 -Wall -Wextra -O2 -s -static solution.cpp -o solution.exe");
        if (result != 0) {
            cerr << "LOI: Khong the bien dich solution.cpp. Ma loi: " << result << endl;
            exit(1); // Thoát nếu biên dịch lỗi
        }
        cerr << "Bien dich solution.cpp thanh cong.\n";
    }

    // <<< HÀM MỚI >>>
    void compile_validator ()
    {
        cerr << "Dang bien dich file kiem tra (validator.cpp).\n";
         // Nên dùng c++17 trở lên và bật cảnh báo
        int result = system("g++ -std=c++14 -Wall -Wextra -O2 -s -static validator.cpp -o validator.exe");
        if (result != 0) {
            cerr << "LOI: Khong the bien dich validator.cpp. Ma loi: " << result << endl;
            exit(1); // Thoát nếu biên dịch lỗi
        }
        cerr << "Bien dich validator.cpp thanh cong.\n";
    }

    // Thêm vào cuối namespace Create trong file create.h

    void compile_checker ()
    {
        cerr << "Dang bien dich file kiem tra (checker.cpp).\n";
        int result = system("g++ -std=c++14 -Wall -Wextra -O2 -s -static checker.cpp -o checker.exe");
        if (result != 0) {
            cerr << "LOI: Khong the bien dich checker.cpp. Ma loi: " << result << endl;
            exit(1); // Thoát nếu biên dịch lỗi
        }
        cerr << "Bien dich checker.cpp thanh cong.\n";
    }

    // Thêm hàm này vào bên trong namespace Create trong file create.h

    // Hàm này đóng gói toàn bộ logic để sinh, chạy và kiểm tra một test case
    bool run_test_case(int iTest, const string& prob_name, ofstream& log_file)
    {
        string input_filename = prob_name + ".inp";
        string output_filename = prob_name + ".out";
        string test_order_str = to_string(iTest);
        string input_gen_time_str = "N/A", input_valid_str = "N/A", validation_time_str = "N/A";
        string solution_error_str = "N/A", solution_time_str = "N/A";
        string checker_time_str = "N/A", checker_feedback_str = "N/A";

        int validation_result = -1, solution_result = -1;

        // --- Sinh Input ---
        if (!freopen(input_filename.c_str(), "w", stdout)) {
             cerr << "Loi: Khong the mo file input " << input_filename << " de ghi cho Test #" << iTest << "." << endl;
             log_file << test_order_str << ",ERROR_OPENING_INP,N/A,N/A,N/A,N/A,N/A,N/A\n";
             return false;
        }

        double input_gen_time, solution_exec_time = -1.0, validation_time = -1.0;
        clock_t start_time;

        start_time = clock();
        Gen::gen(iTest); // Gọi hàm sinh test từ generator.cpp
        input_gen_time = 1000.0 * (clock() - start_time) / CLOCKS_PER_SEC;
        input_gen_time_str = format_time(input_gen_time);
        fclose(stdout);

        // --- KIỂM TRA INPUT BẰNG VALIDATOR ---
        start_time = clock();
        string validator_command = "validator.exe < " + input_filename;
        validation_result = system(validator_command.c_str());
        validation_time = 1000.0 * (clock() - start_time) / CLOCKS_PER_SEC;
        validation_time_str = format_time(validation_time);

        if (validation_result != 0) {
            input_valid_str = "NO";
            cerr << "Test #" << left << setw(4) << iTest << ": FAILED - Input khong hop le (validator tra ve ma loi: " << validation_result << ")." << endl;
            log_file << test_order_str << "," << input_gen_time_str << "," << input_valid_str << "," << validation_time_str << ",N/A,N/A,N/A,N/A\n";
            return false; // Báo hiệu thất bại, nên dừng lại
        } else {
            input_valid_str = "YES";
        }

        // --- Chạy Lời giải ---
        string solution_command = "solution.exe < " + input_filename + " > " + output_filename;
        start_time = clock();
        solution_result = system(solution_command.c_str());
        solution_exec_time = 1000.0 * (clock() - start_time) / CLOCKS_PER_SEC;
        solution_time_str = format_time(solution_exec_time);
        solution_error_str = (solution_result != 0) ? "YES" : "NO";

        // --- Chạy checker ---
        double checker_exec_time = -1.0;
        string checker_feedback_tmp_file = "checker_feedback.tmp";
        string checker_command = "checker.exe " + input_filename + " " + output_filename + " > " + checker_feedback_tmp_file;

        start_time = clock();
        system(checker_command.c_str());
        checker_exec_time = 1000.0 * (clock() - start_time) / CLOCKS_PER_SEC;
        checker_time_str = format_time(checker_exec_time);

        ifstream feedback_stream(checker_feedback_tmp_file);
        if(feedback_stream.is_open()) {
            getline(feedback_stream, checker_feedback_str);
            feedback_stream.close();
            remove(checker_feedback_tmp_file.c_str());
        } else {
            checker_feedback_str = "ERROR: Cannot read feedback file.";
        }

        // --- Sao chép test ---
        // Hàm này sẽ sử dụng biến global 'subtask' được định nghĩa trong template.h
        copy_test(prob_name, subtask, iTest, true);

        // --- Ghi log ---
        log_file << test_order_str << ","
                 << input_gen_time_str << ","
                 << input_valid_str << ","
                 << validation_time_str << ","
                 << solution_error_str << ","
                 << solution_time_str << ","
                 << checker_time_str << ",\""
                 << checker_feedback_str << "\"\n";

        // --- In tóm tắt ra màn hình ---
        cerr << "Test #" << left << setw(4) << iTest
             << ": OK. Timings (ms): "
             << "[Gen: " << setw(7) << input_gen_time_str << "] "
             << "[Val: " << setw(7) << validation_time_str << "] "
             << "[Sol: " << setw(7) << solution_time_str << "] "
             << "[Chk: " << setw(7) << checker_time_str << "]";

        if (solution_result != 0) {
            cerr << " (Solution exited with code " << solution_result << ")";
        }
        cerr << " | Feedback: " << checker_feedback_str << endl;

        return true; // Báo hiệu test này đã chạy thành công
    }
}
