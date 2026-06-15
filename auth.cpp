#include "auth.h"

bool Auth::isValidPassword(string password) {
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char c : password) {
        if (isdigit(c)) hasDigit = true;
        if (!isalnum(c)) hasSpecial = true;
    }

    return hasDigit && hasSpecial;
}

bool Auth::signup(string username, string password) {

    if (!isValidPassword(password)) {
        message = "Password must have number + special char";
        return false;
    }

    ifstream in("users.txt");
    string u, p;

    while (in >> u >> p) {
        if (u == username) {
            message = "User already exists";
            return false;
        }
    }
    in.close();

    ofstream out("users.txt", ios::app);
    out << username << " " << password << endl;
    out.close();

    message = "Signup successful!";
    return true;
}


bool Auth::login(string username, string password) {

    ifstream in("users.txt");
    string u, p;

    while (in >> u >> p) {
        if (u == username && p == password) {
            message = "Login successful!";
            return true;
        }
    }

    message = "Invalid username or password";
    return false;
}