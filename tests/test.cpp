#include <chrono>
#include <filesystem>
#include <vector>
#include <string>

#include "../src/algo.h"

const std::string cacheFolderName = "$AC_TEMP";

namespace fs = std::filesystem;
namespace chrono = std::chrono;

void testIt(char* path){
    std::vector<std::string> fileDirs;


    for(const auto &entry : fs::directory_iterator(path)){
        fileDirs.push_back(entry.path().string());
    }

    fs::current_path(fs::temp_directory_path());
    fs::create_directory(cacheFolderName);
    fs::current_path(cacheFolderName);

    std::string columnNames[] = {"File name", "Encoded size (bytes)", "Decoded size (bytes)", "Encoding time (mu)", "Decoding time (mu)", "Compression rate"};
    for(auto &it: columnNames)
        std::cout << std::setw(25) << std::left << it;
    std::cout << std::endl;
      
    auto start = chrono::steady_clock::now(), end = chrono::steady_clock::now();

    for(int i = 0; i < fileDirs.size(); i++){
        int buffer[4];
        //encode
        std::chrono::milliseconds::zero();
        size_t tempStringSize = fileDirs[i].size();
        int lastSlashPosition = fileDirs[i].find_last_of('\\');
        int lastDotPosition = fileDirs[i].find_last_of('.');
        std::string dumbPath = fs::current_path().string() + fileDirs[i].substr(lastSlashPosition, lastDotPosition-lastSlashPosition);

        std::string fileName =  fileDirs[i].substr(lastSlashPosition+1, fileDirs[i].size()-lastSlashPosition);
        if(fileName.size() > 12){
            fileName = fileName.substr(0, 12) + "...";
        }
        std::cout << std::setw(25) << std::left << fileName; //fileName

        start = chrono::steady_clock::now();
        encode(fileDirs[i].c_str(),dumbPath.c_str());
        end = chrono::steady_clock::now();
        buffer[0] = fs::file_size(dumbPath); //encoded size
        buffer[2] = chrono::duration_cast<chrono::microseconds>(end - start).count(); //encoding time

        //decode
        std::chrono::milliseconds::zero();
        std::string tempDumbPath = dumbPath + ".dec";
        start = chrono::steady_clock::now();
        decode(dumbPath.c_str(),tempDumbPath.c_str());
        end = chrono::steady_clock::now();    
        buffer[1] = fs::file_size(tempDumbPath); //decoded size
        buffer[3] = chrono::duration_cast<chrono::microseconds>(end - start).count(); //decoded time

        for(auto &it: buffer)
            std::cout << std::setw( 25 ) << it;

        std::cout << std::setw( 25 ) << std::fixed << std::setprecision(3) << ((buffer[1]-buffer[0])/(double)buffer[1])*100 << std::endl; //compression rate               
    }

    fs::current_path("..");
    fs::remove_all(cacheFolderName);

}