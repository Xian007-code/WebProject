#include <iostream>
#include <dirent.h>

int main() {
    std::string folderPath = "./files"; // 文件夹路径

    DIR* dir = opendir(folderPath.c_str());
    if (dir == nullptr) {
        std::cerr << "无法打开目录" << std::endl;
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            std::cout << entry->d_name << std::endl;
        }
    }

    closedir(dir);

    return 0;
}

