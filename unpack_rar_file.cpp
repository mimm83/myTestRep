// This program will unpack a rar file and extract the contents to a specified directory.
#include <iostream>
#include <string>
#include <cstdlib>


using namespace std;

void unpack_rar_file(const string& rar_file, const string& output_dir){
    string command = "unrar x " + rar_file + " " + output_dir;
    system(command.c_str());
}


string get_output_dir(const string& rar_file){
    size_t found = rar_file.find_last_of("/\\");
    return rar_file.substr(0, found);
}

// It should be possible to run the program with the following command:
// ./unpack_rar_file -i input.rar

int main(int argc, char* argv[]){
    if (argc != 1) {
        cout << "Usage: " << argv[0] << " <rar_file>" << endl;
        return 1;
    }

    // Give a folder it finds the rar file in the folder
    string rar_file = argv[1];
    string output_dir = get_output_dir(rar_file);

    cout << "Unpacking " << rar_file << " to " << output_dir << endl;
    unpack_rar_file(rar_file, output_dir);

    return 0;
}