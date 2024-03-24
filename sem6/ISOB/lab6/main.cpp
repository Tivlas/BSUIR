#include <algorithm>
#include <boost/regex.hpp>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> NOT_NAMES = {
    "main",
    "include",
    "QSqlDatabase",
    "QString",
    "std",
    "optional",
    "DB",
    "QSqlQuery",
    "size_t",
    "textEdit",
    "MyTextEdit",
    "int",
    "QMimeData",
    "QWidget",
    "QTextEdit",
    "Q_OBJECT",
    "user_actions",
    "Ui",
    "QDirIterator",
    "QDir",
    "Files",
    "QFileInfo",
    "new",
    "SLOT",
    "Subdirectories",
    "qint64",
    "QMessageBox",
    "warning",
    "addItem",
    "next",
    "while",
    "for",
    "do",
    "hasNext",
    "canonicalFilePath",
    "true",
    "false",
    "saveBtn",
    "break",
    "continue",
    "critical",
    "QListWidgetItem",
    "QIODevice",
    "ReadOnly",
    "QTextStream",
    "setPlainText",
    "timeout",
    "readAll",
    "QTimer",
    "setSingleShot",
    "setInterval",
    "connect",
    "QObject",
    "setEnabled",
    "QRect",
    "startsWith",
    "QStringList",
    "deleteLater",
    "start",
    "currentItem",
    "foundFiles",
    "QFile",
    "text",
    "open",
    "WriteOnly",
    "toPlainText",
    "close",
    "insertFromMimeData",
    "user_actions",
    "nullptr",
    "private",
    "public",
    "signals",
    "explicit",
    "static",
    "const",
    "include",
    "ifndef",
    "endif",
    "define",
    "bool",
    "login",
    "setupUi",
    "protected",
    "setHostName",
    "setDatabaseName",
    "QMainWindow",
    "qDebug",
    "setUserName",
    "setPassword",
    "override",
    "once",
    "void",
    "string",
    "hasText",
    "length",
    "addDatabase",
    "return",
    "slots",
    "sign_in",
    "atEnd",
    "sign_out",
    "login_in",
    "password_in",
    "dirPath",
    "size",
    "SIGNAL",
    "delete",
    "isEmpty",
    "auto",
    "if",
    "first",
    "else",
    "value_or",
    "this",
    "emit",
    "value",
    "toInt",
    "prepare",
    "bindValue",
    "exec",
    "class",
    "namespace",
    "QApplication",
    "MainWindow",
    "char",
    "QSize",
    "QT_BEGIN_NAMESPACE",
    "QT_END_NAMESPACE",
    "setCentralWidget",
    "show",
    "hide",
    "centralWidget",
    "setParent",
    "setMaxLength",
    "frame_4",
    "setGeometry",
    "setObjectName",
    "on_pushButton_clicked",
    "on_searchFilesBtn_clicked",
    "on_foundFiles_itemClicked",
    "on_saveBtn_clicked",
    "on_signOutBtn_clicked",
};

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<> distribution(0, 1'000'000);
std::unordered_map<std::string, std::string> VAR_NAMES;

std::string generateRandomName(int length) {
    std::string possibleChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
    std::string randomName;
    do {
        for (int i = 0; i < length; i++) {
            randomName += possibleChars[distribution(generator) % possibleChars.size()];
        }
        if (!std::isalpha(randomName[0]) && randomName[0] != '_') {
            randomName[0] = 'C';
        }
    } while (VAR_NAMES.contains(randomName));
    return randomName;
}

std::string get_cmp_sign(int left_val, int right_val) {
    if (left_val == right_val) {
        std::vector<std::string> cmp = {"==", ">=", "<="};
        return cmp[distribution(generator) % cmp.size()];
    } else if (left_val > right_val) {
        std::vector<std::string> cmp = {"!=", ">=", ">"};
        return cmp[distribution(generator) % cmp.size()];
    } else {
        std::vector<std::string> cmp = {"!=", "<=", "<"};
        return cmp[distribution(generator) % cmp.size()];
    }
}

std::string get_incorrect_cmp_sign(int left_val, int right_val) {
    if (left_val == right_val) {
        return "!=";
    } else if (left_val > right_val) {
        return "<";
    } else {
        return ">";
    }
}

std::string generate_branches(const std::string &var, int val, size_t lvl, size_t max_lvl, bool is_true,
                              const std::string &code, bool &was_pasted, std::vector<bool> path) {
    if (lvl > max_lvl) {
        return "";
    }

    std::string out;
    auto path_if = path, path_else = path;
    path_if.push_back(is_true);
    path_else.push_back(!is_true);

    if (is_true) {
        bool is_next_true = distribution(generator) % 2;

        int if_val = distribution(generator);

        out += std::string(lvl * 4, ' ') + "if (" + var + get_cmp_sign(val, if_val) + std::to_string(if_val) + ") {\n";
        auto vr = generateRandomName(10);
        auto _val = distribution(generator);
        out += std::string((lvl + 1) * 4, ' ') + "int " + vr + " = " + std::to_string(_val) + ";\n";
        out += generate_branches(vr, _val, lvl + 1, max_lvl, is_next_true, code, was_pasted, path_if);

        if (lvl == max_lvl && !was_pasted &&
            static_cast<size_t>(std::count(path.begin(), path.end(), true)) == path.size()) {
            out += std::string(lvl * 4, ' ') + code + '\n';
            was_pasted = true;
        }

        out += std::string(lvl * 4, ' ') + "} else {\n";
        vr = generateRandomName(10);
        _val = distribution(generator);
        out += std::string((lvl + 1) * 4, ' ') + "int " + vr + " = " + std::to_string(_val) + ";\n";
        is_next_true = distribution(generator) % 2;
        out += generate_branches(vr, _val, lvl + 1, max_lvl, is_next_true, code, was_pasted, path_else);
        out += std::string(lvl * 4, ' ') + "}\n";
    } else {
        bool is_next_true = distribution(generator) % 2;

        int if_val = distribution(generator);

        out += std::string(lvl * 4, ' ') + "if (" + var + get_incorrect_cmp_sign(val, if_val) + std::to_string(if_val) +
               ") {\n";
        auto vr = generateRandomName(10);
        auto _val = distribution(generator);
        out += std::string((lvl + 1) * 4, ' ') + "int " + vr + " = " + std::to_string(_val) + ";\n";
        out += generate_branches(vr, _val, lvl + 1, max_lvl, is_next_true, code, was_pasted, path_if);

        out += std::string(lvl * 4, ' ') + "} else {\n";
        if (lvl == max_lvl && !was_pasted &&
            static_cast<size_t>(std::count(path.begin(), path.end(), true)) == path.size()) {
            out += std::string(lvl * 4, ' ') + code + '\n';
            was_pasted = true;
        }
        vr = generateRandomName(10);
        _val = distribution(generator);
        out += std::string((lvl + 1) * 4, ' ') + "int " + vr + " = " + std::to_string(_val) + ";\n";
        is_next_true = distribution(generator) % 2;
        out += generate_branches(vr, _val, lvl + 1, max_lvl, is_next_true, code, was_pasted, path_else);
        out += std::string(lvl * 4, ' ') + "}\n";
    }

    return out;
}

std::string renameVars(const std::string &code) {
    boost::regex pattern(R"((?<![<#\/])(?=(?:[^"']|"[^"]*"|'[^']*')*$)\b([a-zA-Z_]\w*)\b(?![<>]))");
    std::string result = boost::regex_replace(
        code, pattern,
        [&](const boost::smatch &match) {
            std::string varName = match.str(1);
            if (NOT_NAMES.contains(varName)) return match.str(0);
            if (VAR_NAMES.find(varName) == VAR_NAMES.end()) {
                VAR_NAMES[varName] = generateRandomName(10);
            }
            return VAR_NAMES[varName];
        },
        boost::match_default | boost::format_all);

    return result;
}

std::string replaceNumbers(const std::string &code) {
    boost::regex pattern(R"((?<!\.)\b(\d+)\b(?!\.))");
    std::string result = boost::regex_replace(
        code, pattern,
        [&](const boost::smatch &match) {
            std::string result;
            int number = std::stoi(match.str(1));
            int cur = 0;
            int rem = number + 1;
            int maxOpCnt = distribution(generator) % 4 + 2;
            while (cur != number) {
                if (maxOpCnt == 0) {
                    result += "0x" + std::format("{:x}", rem - 1) + " + ";
                    break;
                }
                int tmp = distribution(generator) % rem;
                result += "0x" + std::format("{:x}", tmp) + " + ";
                cur += tmp;
                rem -= tmp;
                maxOpCnt--;
            }
            result += "0x0";
            return "(" + result + ")";
        },
        boost::match_default | boost::format_all);

    return result;
}

std::string insertIfElse(const std::string &code) {
    boost::regex pattern(R"((?<!\w\s)(\{)([^{}]+)(\}))");
    std::string result = boost::regex_replace(
        code, pattern,
        [&](const boost::smatch &match) {
            auto code = match.str(2);
            bool was_pasted = false;
            code = generate_branches("a", 100, 0, 2, true, code, was_pasted, {});
            return match.str(1) + "  int a = 0xaB1f * 0xBc94 - 0x7e0db1EC;" + code + match.str(3);
        },
        boost::match_default | boost::format_all);
    return result;
}

std::string obfuscate(const std::string &code) {
    std::string result;
    result = renameVars(code);
    result = insertIfElse(result);
    result = replaceNumbers(result);
    return result;
}

std::vector<std::filesystem::path> getFileNames(const std::filesystem::path &dataDir) {
    std::vector<std::filesystem::path> fileNames;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(dataDir)) {
        if (entry.is_regular_file() && (entry.path().extension() == ".cpp" || entry.path().extension() == ".h")) {
            std::filesystem::path filePath = std::filesystem::absolute(entry.path()).lexically_normal();
            fileNames.push_back(filePath);
        }
    }
    return fileNames;
}

std::string readFile(const std::filesystem::path &path) {
    std::string comm = "clang-format -i " + path.string();
    system(comm.c_str());
    auto file = std::ifstream(path);
    std::string code;
    std::string line;
    while (std::getline(file, line)) {
        code += line + '\n';
    }
    file.close();
    return code;
}

int main() {
    auto fileNames = getFileNames("lab-files");
    for (const auto &path : fileNames) {
        auto code = readFile(path);
        std::string obfuscated_code = obfuscate(code);
        std::string from = "lab-files";
        std::string to = "lab-files-copy";
        size_t replacePos = path.string().find(from);
        auto outPath = std::filesystem::path(path.string().replace(replacePos, from.size(), to));
        std::ofstream out(outPath);
        out << obfuscated_code;
        out.close();
        std::string comm = "clang-format -i " + outPath.string();
        system(comm.c_str());
    }

    return 0;
}