#include <iostream>

#include "FileWatcher/FileWatcher.h"
#include "ChecksumAPI/SHA256.h"

void fileWatcherTest () {
    std::cout << "Enter path to watch: ";
    std::string path_to_watch;
    std::cin>>path_to_watch;

    // Create a Connection
    Connection conn_("0.0.0.0", 5007, path_to_watch);
    conn_.send_string("login guido guido.poli");
    conn_.read_string();

    // Create a FileWatcher instance that will check the current folder for changes every 5 seconds
    FileWatcher fw{path_to_watch, std::chrono::milliseconds(5000), conn_};

    // Start monitoring a folder for changes and (in case of changes)
    // run a user provided lambda function
    fw.start([] (const std::string& file_path, Connection& conn_, FileStatus status) -> void {
        // Process only regular files, all other file types are ignored
        if(!std::filesystem::is_regular_file(std::filesystem::path(file_path))) {
            return;
        }

        switch(status) {
            case FileStatus::created:
                conn_.send_file(file_path);
                std::cout << "File created: " << file_path << '\n';
                break;
            case FileStatus::modified:
                std::cout << "File modified: " << file_path << '\n';
                break;
            case FileStatus::erased:
                std::cout << "File erased: " << file_path << '\n';
                break;
            default:
                std::cout << "Error! Unknown file status.\n";
        }
    });
}

void socketTest(){
    Connection s("0.0.0.0", 5007, "/Users/andreascopp/Desktop/Client-TestFiles/");
    std::cout << s.read_string(); //Leggo quello che mi arriva appena instauro la connessione
    std::string message;
    while(message != "stop") {
        std::getline(std::cin, message);
        s.send_string(message);
        std::cout << s.read_string();
    }

    //Per usare il metodo prova col server di prova
    /*Connection s;
    s.prova("0.0.0.0", 1234);*/
}

void sendFileTest(){
    Connection s("0.0.0.0", 5002, "/Users/andreascopp/Desktop/Client-TestFiles/");
    std::string input;
    while (input != "y" && input != "n") {
        std::cout << "Do you want to send the file now?(y/n): ";
        std::getline(std::cin, input);
        if (input == "y") {
            s.send_file("/Users/andreascopp/Desktop/Client-TestFiles/invio_client_grande.txt");
        }
        else if (input == "n"){
            std::cout << "Ok bye!" << std::endl;
        }
        else
            std::cout << "Enter a valid selection!" << std::endl;
    }
}

void sendFile2(){
    Connection s("0.0.0.0", 5002, "/Users/andreascopp/Desktop/Client-TestFiles/");
    s.send_string("login guido guido.poli");
    s.read_string();
    s.send_file("/Users/andreascopp/Desktop/Client-TestFiles/invio_client_grande.txt");
}

void readFileTest(){
    Connection s("0.0.0.0", 1234, "/Users/andreascopp/Desktop/Client-TestFiles/");
    std::string input;
    s.read_file();
}

void checksum(){
    std::string checksum = sha256("prova");
    std::cout << "Checksum: " << checksum << std::endl; //FIXME è diverso da quello di openSSL
}

int main() {
    std::cout << "Menu options:" << std::endl;

    // Add new options as needed
    std::cout << "0 - fileWatcherTest" << std::endl;
    std::cout << "1 - socketTest" << std::endl;
    std::cout << "2 - sendFileTest" << std::endl;
    std::cout << "3 - readFileTest" << std::endl;
    std::cout << "4 - checksumTest" << std::endl;
    std::cout << "5 - sendFile2" << std::endl;

    std::cout << "Enter selection: ";

    // Read the input
    std::string input;
    std::getline(std::cin, input);
    int selection = std::stoi(input);
    switch(selection){
        case 0:
            std::cout << "FileWatcher Test Initialized" << std::endl;
            fileWatcherTest();
            break;
        case 1:
            std::cout << "Socket Test Initialized" << std::endl;
            socketTest();
            break;
        case 2:
            std::cout << "Send File Test Initialized" << std::endl;
            sendFileTest();
            break;
        case 3:
            std::cout << "Read File Test Initialized" << std::endl;
            readFileTest();
            break;
        case 4:
            std::cout << "Checksum Test Initialized" << std::endl;
            checksum();
            break;
        case 5:
            std::cout << "Send File 2 Test Initialized" << std::endl;
            sendFile2();
            break;
        default:
            std::cout << "Error!" << std::endl;
    }
    return 0;
}