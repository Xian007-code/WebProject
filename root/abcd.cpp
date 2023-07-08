#include <iostream>
#include <dirent.h>

std::string get_file_list(const std::string& folderPath) {
    std::string fileList;

    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(folderPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) { // 只考虑常规文件
                fileList += ent->d_name;
                fileList += '\n';
            }
        }
        closedir(dir);
    }

    return fileList;
}


int main() {
    std::string fileList = get_file_list("./files");
    std::cout << fileList << std::endl;


    return 0;
}

