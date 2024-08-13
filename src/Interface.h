#pragma once
#include <exception>
#include <iostream>
#include <istream>
#include <limits>
#include <string>

#include "NoteManager.h"

class Interface {
 private:
  NoteManager manager;      // Manages the notes
  std::string name;         // Stores the name of the note
  std::string description;  // Stores the description of the note
  char choice;              // Stores the user's menu choice

 public:
  Interface(const std::string& filePath) : choice(' '), manager(filePath) {}

  void run() {
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
      std::cin >> choice;                                                  // Read user's choice
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore remaining input

      switch (choice) {
        case '1':  // Add Note
          std::cout << "Enter the note's name: ";
          std::getline(std::cin, name);  // Read the note's name
          std::cout << "Enter the note's description: ";
          std::getline(std::cin, description);  // Read the note's description

          // Default to "blank" if the user input is empty
          name = name.empty() ? "blank" : name;
          description = description.empty() ? "blank" : description;

          manager.addNote(std::move(name), std::move(description));  // Add the note to the manager
          std::cout << "==================" << std::endl;
          break;

        case '2':  // Delete Note
          if (manager.isEmpty()) {
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
          }

          std::cout << "Enter the name of the note to delete: ";
          std::getline(std::cin, name);          // Read the name of the note to delete
          name = name.empty() ? "blank" : name;  // Default to "blank" if input is empty

          try {
            manager.deleteNote(std::move(name));  // Attempt to delete the note
          } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;  // Print error message if deletion fails
          }
          std::cout << "==================" << std::endl;
          break;

        case '3':  // Change Note Description
          if (manager.isEmpty()) {
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
          }

          std::cout << "Enter the name of the note to modify: ";
          std::getline(std::cin, name);  // Read the name of the note to modify

          name = name.empty() ? "blank" : name;                       // Default to "blank" if input is empty
          description = description.empty() ? "blank" : description;  // Default to "blank" if input is empty

          try {
            manager.changeDescription(std::move(name));  // Attempt to change the description of the note
          } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;  // Print error message if modification fails
          }
          std::cout << "==================" << std::endl;
          break;

        case '4':  // Change Note Status
          if (manager.isEmpty()) {
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
          }

          std::cout << "Enter the name of the note to modify: ";
          std::getline(std::cin, name);          // Read the name of the note to modify
          name = name.empty() ? "blank" : name;  // Default to "blank" if input is empty

          try {
            manager.changeStatus(std::move(name));  // Attempt to change the status of the note
          } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;  // Print error message if modification fails
          }
          std::cout << "==================" << std::endl;
          break;

        case '5':
          manager.printNotes();  // Print all notes
          std::cout << "==================" << std::endl;
          break;

        case '6':  // Exit
          return;

        default:
          std::cout << "Invalid option. Please try again." << std::endl;  // Handle invalid menu choices

          std::cout << "==================" << std::endl;
          break;
      }
    }
  }
};