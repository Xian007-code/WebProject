#include<iostream>
#include<dirent.h>

using namespace std;

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

int main()
{
	string a=get_file_list("pictures");
	cout<<a<<endl;
}

