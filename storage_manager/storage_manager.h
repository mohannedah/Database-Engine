#pragma once
#include<bits/bits-stdc++.h>
#include <json/json.h>
using namespace std;
namespace StorageManagerNameSpace {

    class StorageManager {
    private:

        static const Json::Value getFromFile(ifstream *file, int offset, int endingOffset);
        static int storeInFile(ofstream *file, const Json::Value& jsonObject);
        
    public:
        static const Json::Value get(ifstream *file, int offset, int endOffset);
        static bool exists(string& filePath);
        static int put(ofstream *file, const Json::Value& jsonObject);
        static void closeReadFile(ifstream* file);
        static void closeWriteFile(ofstream* file);
        static ofstream* openWrite(string filePath);
        static ifstream* openRead(string filePath);
        static int getEndOffset(string filePath);
        static int removeFile(string filePath);


        template<typename T>
   
        static void closeFile(T* file);
    };


}
