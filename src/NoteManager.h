#pragma once
#include <algorithm>
#include <cstddef>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Note.h"

// Comparator to sort notes by their completion status
struct CompareByCompletion {
  bool operator()(const Note& a, const Note& b) const {
    return a.getStatus() < b.getStatus();
  }
};

class NoteManager {
 private:
  const std::string PATH;   // Path to the file where notes are stored
  std::vector<Note> notes;  // Vector to store the notes
  std::unordered_map<std::string, size_t>
      noteIndex;  // Map to quickly find a note by its name

  // Method to trim leading and trailing whitespace from a string
  std::string trim(const std::string& str) const {
    const auto start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    const auto end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
  }

  // Method to sort the notes by their completion status
  void sortNotesByCompletion() {
    std::sort(notes.begin(), notes.end(), CompareByCompletion());
  }

  int findNoteIndex(const std::string& name) {
    auto it = noteIndex.find(name);
    if (it == noteIndex.end()) {
      throw std::invalid_argument(std::format("Note '{}' not found", name));
    }

    return it->second;
  }

 public:
  // Constructor to initialize the NoteManager with the path to the file
  explicit NoteManager(const std::string& path) : PATH(path) {}
  // Method to read the notes from the file
  void readFromFile() {
    notes.clear();
    noteIndex.clear();
    std::ifstream file(PATH);
    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file for reading.");
    }

    std::string line;
    size_t index = 0;
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string name, description, statusStr;
      std::getline(ss, name, ':');
      std::getline(ss, description, ':');
      std::getline(ss, statusStr, ':');

      name = trim(name);
      description = trim(description);
      statusStr = trim(statusStr);

      bool complete = (statusStr == "Complete");
      notes.emplace_back(std::move(name), std::move(description), complete);
      noteIndex[name] = index++;
    }
    file.close();
  }

  // Method to save the notes to the file
  void safeToFile() const {
    std::ofstream file(PATH);
    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file for writing.");
    }

    for (const auto& note : notes) {
      file << note.getName() << ": " << note.getDescription() << ": "
           << (note.getStatus() ? "Complete" : "Not complete") << std::endl;
    }
    file.close();
  }
  // Method to add a new note
  void addNote(const std::string& name, const std::string& description) {
    bool complete = false;

    auto it = noteIndex.find(name);

    if (it != noteIndex.end()) {
      std::cout
          << "Note with this name already exists. What would you like to do?"
          << std::endl;
      std::cout << "Enter 'u' to update the existing note or 'q' to quit: ";
      std::string choice;
      std::getline(std::cin, choice);

      if (choice == "q") return;

      if (choice == "u") {
        notes[it->second].setDescription(description);
      } else {
        std::cout << "Note not modified." << std::endl;
      }
    } else {
      notes.emplace_back(std::move(name), std::move(description), complete);
      noteIndex[name] = notes.size() - 1;
    }
    sortNotesByCompletion();
  }

  // Method to delete a note
  void deleteNote(const std::string& name) {
    try {
      int index = findNoteIndex(name);
      notes.erase(notes.begin() + index);
      noteIndex.erase(name);

      // Update indices of the remaining notes
      for (std::size_t i = index; i < notes.size(); ++i) {
        noteIndex[notes[i].getName()] = i;
      }

      std::cout << std::format("Note '{}' was deleted", name) << std::endl;
    } catch (const std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  // Method to change the description of a note
  void changeDescription(const std::string& name) {
    try {
      int index = findNoteIndex(name);
      std::string description;
      std::cout << "Enter the new description: ";
      std::getline(std::cin, description);
      description = description.empty() ? "blank" : description;
    } catch (const std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  // Method to change the completion status of a note
  void changeStatus(const std::string& name) {
    try {
      int index = findNoteIndex(name);
      notes[index].setStatus(!notes[index].getStatus());
      sortNotesByCompletion();
    } catch (const std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  // Method to print all the notes
  void printNotes() {
    if (notes.empty()) {
      std::cout << "No notes available" << std::endl;
    } else {
      for (const auto& note : notes) {
        std::cout << note << std::endl;
      }
    }
  }

  bool isEmpty() const { return notes.empty(); }
};
