#include <string>
#include "GenQueue.h"
#include "Window.h"
using namespace std;

class Registrar{
  public:
    Registrar();
    ~Registrar();

    void setNumWindows(int numWindows);

    void assignWindow(Student* s);
    void checkTime(int index, int currTick);
    bool checkOpen();
    bool isEmpty();
    bool isFull();

    Window* windows;

  private:
    int m_numWindows;
    int occupiedWindows;

};
