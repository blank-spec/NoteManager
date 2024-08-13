#pragma once
#include <istream>
#include <string>

class Note {
 private:
  std::string name;         // Name of the note
  std::string description;  // Description of the note
  bool complete;            // Status of the note (complete or not)

 public:
  // Constructor to initialize the note with name, description, and optional
  // status
  Note(const std::string& name, const std::string& description, bool complete = false)
      : name(std::move(name)),                // Move name to avoid unnecessary copy
        description(std::move(description)),  // Move description to avoid unnecessary copy
        complete(complete) {}                 // Initialize status

  // Getter for the note's name
  const std::string& getName() const { return name; }

  // Getter for the note's status
  bool getStatus() const { return complete; }

  // Getter for the note's description
  const std::string& getDescription() const { return description; }

  // Setter for the note's status
  void setStatus(bool status) { complete = status; }

  // Setter for the note's name
  void setName(const std::string& name) { this->name = name; }

  // Setter for the note's description
  void setDescription(const std::string& description) {
    this->description = description;
  }

  // Overload the output operator to print the note details
  friend std::ostream& operator<<(std::ostream& os, const Note& note);
};

// Definition of the overloaded output operator
inline std::ostream& operator<<(std::ostream& os, const Note& note) {
  os << note.getName() << ": " << note.getDescription() << ": "
     << (note.getStatus() ? "Complete" : "Not complete");  // Print status based on its value
  return os;
}
