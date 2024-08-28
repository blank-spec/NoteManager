#pragma once
#include <exception>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <string>
#include <unordered_map>

#include "NoteManager.h"

using namespace std;

class Interface {
 private:
  NoteManager manager;      // Manages the notes
  std::string name;         // Stores the name of the note
  std::string description;  // Stores the description of the note
  char choice;              // Stores the user's menu choice

 public:
  Interface(const std::string& filePath) : choice(' '), manager(filePath) {}

  void run() {
    manager.readFromFile();
    std::unordered_map<char, std::function<void()>> actions = {
        {'1',
         [&]() {
           std::cout << "Enter the note's name: ";
           std::getline(std::cin, name);
           std::cout << "Enter the note's description: ";
           std::getline(std::cin, description);

           name = name.empty() ? "blank" : name;
           description = description.empty() ? "blank" : description;

           manager.addNote(std::move(name), std::move(description));
           std::cout << "==================" << std::endl;
         }},
        {'2',
         [&]() {
           if (manager.isEmpty()) {
             std::cout << "Invalid option. Please try again." << std::endl;
             return;
           }
           std::cout << "Enter the name of the note to delete: ";
           std::getline(std::cin, name);
           name = name.empty() ? "blank" : name;

           try {
             manager.deleteNote(std::move(name));
           } catch (const std::exception& e) {
             std::cout << e.what() << std::endl;
           }
           std::cout << "==================" << std::endl;
         }},
        {'3',
         [&]() {
           if (manager.isEmpty()) {
             std::cout << "Invalid option. Please try again." << std::endl;
             return;
           }
           std::cout << "Enter the name of the note to modify: ";
           std::getline(std::cin, name);

           name = name.empty() ? "blank" : name;
           description = description.empty() ? "blank" : description;

           try {
             manager.changeDescription(std::move(name));
           } catch (const std::exception& e) {
             std::cout << e.what() << std::endl;
           }
           std::cout << "==================" << std::endl;
         }},
        {'4',
         [&]() {
           if (manager.isEmpty()) {
             std::cout << "Invalid option. Please try again." << std::endl;
             return;
           }
           std::cout << "Enter the name of the note to modify: ";
           std::getline(std::cin, name);
           name = name.empty() ? "blank" : name;

           try {
             manager.changeStatus(std::move(name));
           } catch (const std::exception& e) {
             std::cout << e.what() << std::endl;
           }
           std::cout << "==================" << std::endl;
         }},
        {'5',
         [&]() {
           manager.printNotes();
           std::cout << "==================" << std::endl;
         }},
        {'6', [&]() {
           manager.safeToFile();
           exit(0);  // Exit from the program
         }}};

    while (true) {           // Infinite loop to keep the interface running
      manager.printNotes();  // Print all notes
      std::cout << "\nMenu:" << std::endl;
      std::cout << "1. Add Note" << std::endl;

      if (!manager.isEmpty()) {
        std::cout << "2. Delete Note" << std::endl;
        std::cout << "3. Change Note Description" << std::endl;
        std::cout << "4. Change Note Status" << std::endl;
      }
      std::cout << "5. Print All Notes" << std::endl;
      std::cout << "6. Exit" << std::endl;
      std::cout << "Choose an option: ";
      std::cin >> choice;  // Read user's choice
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n');  // Ignore remaining input

      if (actions.find(choice) != actions.end()) {
        actions[choice]();
      } else {
        std::cout << "Invalid option. Please try again." << std::endl;
        std::cout << "==================" << std::endl;
      }
    }
  }
};
