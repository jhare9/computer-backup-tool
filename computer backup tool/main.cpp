//
// main.cpp
// Computer Backup
//
// Created by jonathon hare on 1/15/16.
// Copyright © 2016 jonathon hare. All rights reserved.
//

// header files needed.
#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stack>

using namespace std;

bool is_not_visted(stack<string> pased_stack,string target);


int main(int argc, const char * argv[]) {
    
    // variables needed
    stack<string> dir;
    stack<string> visted_dir;
    DIR *dir_ptr;
    struct dirent *dire;
    struct stat st = {0};
    ifstream folder_to_copy_from;
    ofstream folder_to_copy_to;
    string start = "";
    string cpy_path ="";
    
    // used to check if the command line arguments are a corrected file path.
    for(int i = 1; i < argc; i++){
        string temp_args = argv[i];
        // file paths have / in them.
        if(temp_args.find("/") != string::npos){
            // check to see if it is a directory to read in and copy to.
            if((dir_ptr = opendir(argv[i])) != NULL){
                switch(i){
                    case 1:
                        start = argv[i];
                        break;
                    case 2:
                        cpy_path = argv[i];
                        break;
                }
                closedir(dir_ptr);
            }else{
                cout<<"please enter a valid file path ex /User/Username/Documents\n";
                return 1;
            }
        }else{
            cout<<"please enter a valid file path ex /User/Username/Documents\n";
            return 1;
        }
    }
    
    // push the first file path to the stacks
    dir.push(start);
    visted_dir.push(start);
    
    // loop through the stack untill it is empty
    while(!dir.empty()){
        // set the path
        string path = dir.top();
        // open the directory to copy
        dir_ptr = opendir(path.c_str());
        //pop the directroy out of the stack we already used it.
        dir.pop();
        
        // make sure the dir_ptr is not  NUll.
        if(dir_ptr != NULL){
            //get the files and directorys in the paths
            while((dire = readdir(dir_ptr)) != NULL){
                // set a temporary directory
                DIR *temp_dir;
                // get the name of the files and directorys and add it to the file path
                string convert_path = (string)path+"/"+dire->d_name;
                string temp_copy_path = convert_path;
                // set the path to copy the files and directorys to
                temp_copy_path.replace(0,start.length(),cpy_path.c_str());
                
                //we don't want to go up a level we want to stay in the same directory.
                if(convert_path[convert_path.length()-1] != '.'){
                    
                    // check to see if the path has been visted.
                    if((is_not_visted(visted_dir,dire->d_name))){
                        
                        // check to see if it is a directory if not its a file.
                        if((temp_dir = opendir(convert_path.c_str())) != NULL){
                            
                            //put the directories to vist in the stack
                            dir.push(convert_path);
                            visted_dir.push(convert_path);
                            
                            //check to see if the directory already exist. if not create it.
                            if(stat(temp_copy_path.c_str(),&st) == -1){
                                cout<<"creating dir for "<<temp_copy_path<<"\n";
                                mkdir(temp_copy_path.c_str(), 0700);
                            }
                            
                            closedir(temp_dir);
                            
                            // else it is a file so read the files in and write them to a diffrent file path.
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

// checks to see if the target if the target directory or file is in the stack if so it is visted.
bool is_not_visted(stack<string> pased_stack,string target){
    
    stack<string> temp_stack = pased_stack;
    string check ="";
    while(!temp_stack.empty()){
        check = temp_stack.top();
        if(check.find(target) != string::npos)
            return false;
        
        temp_stack.pop();
        
    }
    
    return true;
}

