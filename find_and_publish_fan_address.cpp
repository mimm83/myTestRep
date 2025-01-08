// This program is used to find assigned fan address and fix the conky configuration file
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

string find_dell_smm_in_output(const string& input);
string run_ls_on_hwmon();
void update_conky_config(const string& hwmon);

int main(){
    string fan_address = run_ls_on_hwmon();
    string dell_smm = find_dell_smm_in_output(fan_address);
    cout << dell_smm << endl;
    update_conky_config(dell_smm);
    return 0;
}

string find_dell_smm_in_output(const string& input){
    istringstream stream(input);
    string line;

    while(getline(stream, line)){
        // Check if the line contains "dell_smm"
        if (line.find("dell_smm") != std::string::npos) {
            // Find the hwmon number in the line
            size_t hwmonPos = line.find("hwmon");
            if (hwmonPos != std::string::npos) {
                // Extract and return the hwmon numbers
                return line.substr(hwmonPos + 5, 1);
            }
        }
    }
    return "";
}

string run_ls_on_hwmon(){
    // Run terminal command to get fan address
    string fan_address = "ls -l /sys/class/hwmon";

    // Open terminal and run the command
    FILE *fp = popen(fan_address.c_str(), "r");

    if (fp == NULL) {
        cout << "Failed to run command" << endl;
        exit(1);
    }
    // print the output of the command
    char buffer[1024];
    string result = "";
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        result += buffer;
    }
    pclose(fp);

    return result;
}

void update_conky_config(const string& hwmon){
    // Function will open the conky configuration file and update the fan address
    // the hwmon is a string number for instance "0"
    string conky_config_path = "/etc/conky/conky.conf";
    ifstream conky_config_file(conky_config_path);
    string line;
    vector<string> lines;
    while(getline(conky_config_file, line)){
        if (line.find("RPM") != string::npos) {
            size_t hwmonPos = line.find("hwmon");
            if (hwmonPos != string::npos) {
                // Extract and return the hwmon numbers
                line.replace(hwmonPos + 6, 1, hwmon);
            }
        }
        lines.push_back(line);
    }
    conky_config_file.close();
    // Open the file for writing to save the updated content
    ofstream output_file(conky_config_path);
    if (!output_file.is_open()) {
        cerr << "Error: Could not open file for writing: " << conky_config_path << endl;
        return;
    }

    for (const auto& modified_line : lines) {
        output_file << modified_line << "\n";
    }
    output_file.close();
}