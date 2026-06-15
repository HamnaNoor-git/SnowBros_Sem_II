#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Auth {
public:
    string message;   

    bool signup(string username, string password);
    bool login(string username, string password);
    bool isValidPassword(string password);
};