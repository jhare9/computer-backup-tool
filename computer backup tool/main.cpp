//
// main.cpp
// Computer Backup
//
// Created by jonathon hare on 1/15/16.
// Copyright © 2016 jonathon hare. All rights reserved.
//

#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stack>
#include <sstream>

using namespace std;

bool is_visted(stack<string> pased_stack,string target);


int main(int argc, const char * argv[]) {
    
    stack<string> dir;
    stack<string> visted_dir;
    DIR *dir_ptr;
    struct dirent *dire;
    struct stat st = {0};
    ifstream folder_to_copy_from;
    ofstream folder_to_copy_to;
    
    string start = "/Users/jonathon/Downloads";
    dir.push(start);
    visted_dir.push(start);
    
    string cpy_path = "/Users/jonathon/Documents/copy_files";
    
    while(!dir.empty()){
        string path = dir.top();
        dir_ptr = opendir(path.c_str());
        dir.pop();
        
        if(dir_ptr != NULL){
            while((dire = readdir(dir_ptr)) != NULL){
                
                DIR *temp_dir;
                string convert_path = (string)path+"/"+dire->d_name;
                string temp_copy_path = convert_path;
                temp_copy_path.replace(0,start.length(),cpy_path.c_str());
                
                if(convert_path[convert_path.length()-1] != '.'){
                    
                    if((!is_visted(visted_dir,dire->d_name))){
                        
                        if((temp_dir = opendir(convert_path.c_str())) != NULL){
                            
                            dir.push(convert_path);
                            visted_dir.push(convert_path);
                            
                            if(stat(temp_copy_path.c_str(),&st) == -1){
                                cout<<"creating dir for "<<temp_copy_path<<"\n";
                                mkdir(temp_copy_path.c_str(), 0700);
                            }
                            
                            closedir(temp_dir);
                            
                        }else{
                            cout<<"writing file from "<<convert_path<<" to "<<temp_copy_path<<"\n";
                            folder_to_copy_from.open(convert_path);
                            folder_to_copy_to.open(temp_copy_path);
                            folder_to_copy_to << folder_to_copy_from.rdbuf();
                            folder_to_copy_to.flush();
                            folder_to_copy_from.close();
                            folder_to_copy_to.close();
                        }
                    }
                }
                
            }
            
        }
        closedir(dir_ptr);
    }
    
    return 0;
}

//functons

bool is_visted(stack<string> pased_stack,string target){
    
    stack<string> temp_stack = pased_stack;
    string check ="";
    while(!temp_stack.empty()){
        check = temp_stack.top();
        if(check.find(target) != string::npos)
            return true;
        
        temp_stack.pop();
        
    }
    return false;
}

