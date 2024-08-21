#include <string>

#include "src/Interface.h"

using namespace std;

int main() {
  Interface ui("Path/To/Your/Note.txt");
  ui.run();

  return 0;
}
