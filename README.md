# NoteManager

`NoteManager` is a simple note management application. It allows you to add, delete, and modify notes, as well as toggle their completion status. Notes are saved in a text file.

## Description

`NoteManager` provides functionality for:
- Adding new notes or updating existing ones.
- Deleting notes.
- Changing the description of notes.
- Toggling the completion status of notes.
- Printing all notes.

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/blank-spec/NoteManager
    ```

2. Navigate to the project directory:
    ```bash
    cd NoteManager
    ```

3. Create a build directory and build the project:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. Run the project:
    ```bash
    ./NoteManager
    ```

## Configuration

Make sure to set the path for the note file in the `PATH` variable in the `NoteManager` class. The path should point to a file that ends with `Note.txt`. For example:
```cpp
const std::string PATH = "C:/Project/Note.txt";
