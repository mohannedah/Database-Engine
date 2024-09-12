// storage_manager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "storage_manager.h"


namespace StorageManagerNameSpace {
    const Json::Value StorageManager::getFromFile(ifstream *file, int offset, int endingOffset) {
        file->seekg(offset);
        string stringifiedRecord;
        vector<char> chrs(endingOffset - offset);
        file->read(chrs.data(), endingOffset - offset);

        for (auto chr : chrs) stringifiedRecord += chr;

        Json::Value record;
        Json::CharReaderBuilder reader;
        string errors;
        istringstream iss(stringifiedRecord);

        Json::parseFromStream(reader, iss, &record, &errors);
        return record;
    }


    int StorageManager::storeInFile(ofstream *file, const Json::Value& jsonObject) {
        file->seekp(0, ios::end);
        int offset = file->tellp();
        *file << jsonObject.toStyledString(); 
        file->flush();
        return offset; 
    }
    
    
    void StorageManager::closeReadFile(ifstream* file) {
        file->close();
        delete file;
    }

    void StorageManager::closeWriteFile(ofstream* file) {
        file->close();
        delete file;
    }

    ofstream* StorageManager::openWrite(string filePath) {
       ofstream* ptr = new ofstream(filePath, std::ios::app | std::ios::binary);
       return ptr;
    }

    ifstream* StorageManager::openRead(string filePath)
    {
      ifstream* ptr = new ifstream(filePath, ios::binary);
      return ptr;
    }

    int StorageManager::getEndOffset(string filePath) {
        ifstream file(filePath, ios::binary);
        file.seekg(0, ios::end);
        return file.tellg();
    }

    int StorageManager::removeFile(string filePath)
    {
        ofstream ofs;
        ofs.open(filePath, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
        return 1;
    }


    const Json::Value StorageManager::get(ifstream *file, int offset, int endOffset) {
        Json::Value record;

        if (offset == -1) return record;

        return getFromFile(file, offset, endOffset);
    }


    bool StorageManager::exists(string &filePath) {
        ifstream f(filePath.c_str());
        return f.good();
    };

    int StorageManager::put(ofstream *file, const Json::Value& jsonObject) {
        return storeInFile(file, jsonObject);
    }
};


