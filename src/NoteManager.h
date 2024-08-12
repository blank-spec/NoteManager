#pragma once
#include <algorithm>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <istream>
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
  auto operator()(const Note& a, const Note& b) const -> bool {
    return a.getStatus() < b.getStatus();
  }
};

class NoteManager {
 private:
  // Path to the file where notes are saved
  const std::string PATH;
  // Container to store notes
  std::vector<Note> notes;
  // Map to track the index of each note by its name
  std::unordered_map<std::string, size_t> noteIndex;

  // Save all notes to the file
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

  // Trim leading and trailing whitespace from a string
  auto trim(const std::string& str) const -> std::string {
    const auto start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    const auto end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
  }

  // Read notes from the file and load them into the vector
  void readFromFile() {
    notes.clear();
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

  // Sort notes by their completion status
  void sortNotesByCompletion() {
    std::sort(notes.begin(), notes.end(), CompareByCompletion());
  }

 public:
  NoteManager(const std::string& path) : PATH(path) {}
  // Add a new note or update an existing one
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
        std::cout << "Note not modified.\n";
      }
    } else {
      notes.emplace_back(std::move(name), std::move(description), complete);
      noteIndex[name] = notes.size() - 1;
    }
    sortNotesByCompletion();
    safeToFile();
  }

  // Delete a note by its name
  void deleteNote(const std::string& name) {
    auto it = noteIndex.find(name);
    if (it != noteIndex.end()) {
      size_t index = it->second;
      notes.erase(notes.begin() + index);
      noteIndex.erase(it);

      // Update indices of the remaining notes
      for (size_t i = index; i < notes.size(); ++i) {
        noteIndex[notes[i].getName()] = i;
      }

      std::cout << std::format("Note '{}' was deleted", name) << std::endl;
      safeToFile();
    } else {
      const std::string error = std::format("Note '{}' not found", name);
      throw std::invalid_argument(std::move(error));
    }
  }

  // Change the description of an existing note
  void changeDescription(const std::string& name) {
    auto it = noteIndex.find(name);
    if (it != noteIndex.end()) {
      std::string description;
      std::cout << "Enter the new description: ";
      std::getline(std::cin, description);
      description = description.empty() ? "blank" : description;
      notes[it->second].setDescription(description);
      safeToFile();
    } else {
      const std::string error = std::format("Note '{}' not found", name);
      throw std::invalid_argument(std::move(error));
    }
  }

  // Toggle the completion status of a note
  void changeStatus(const std::string& name) {
    auto it = noteIndex.find(name);
    if (it != noteIndex.end()) {
      notes[it->second].setStatus(!notes[it->second].getStatus());
      sortNotesByCompletion();
      safeToFile();
    } else {
      const std::string error = std::format("Note '{}' not found", name);
      throw std::invalid_argument(std::move(error));
    }
  }

  // Print all notes
  void printNotes() {
    readFromFile();
    if (notes.empty()) {
      std::cout << "No notes available" << std::endl;
    } else {
      for (const auto& note : notes) {
        std::cout << note << std::endl;
      }
    }
  }
};
