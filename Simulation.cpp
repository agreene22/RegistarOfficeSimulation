#include "Simulation.h"
#include <fstream>

Simulation::Simulation(){
  m_meanStudentWait = 0.0;
  m_medianStudentWait = 0.0; // store waits in an array and find median?
  m_longestStudentWait = 0.0;
  m_studentsOverTen = 0.0;
  m_meanWindowIdle = 0.0;
  m_longestWindowIdle = 0.0;
  m_windowsIdleOver5 = 0.0;
  queue = new GenQueue<Student>(10); // we need to be able to make this the correct size
  students = new DoublyLinkedList<Student>();
}

Simulation::~Simulation(){
  delete queue;
  delete students;
}

void Simulation::Run(string fileName){
  // GenQueue<Student>* queue = new GenQueue<Student>(10);

  Registrar* office = new Registrar();
  // Student* first;
  ifstream inFS;

  float clockTick = 0;
  float nextClockTickLine = 1.0;
  float lineCount = 0.0;

  float windowsOpen = 0.0;
  float time = 0.0;
  float numStudents = 0.0;
  float studentTime = 0.0;
  // float windowsOccupied = 0.0;

  float totalTime = 0.0;

  inFS.open(fileName);

  if(!inFS.is_open()){
    cout << "Error: Could not open file." << endl;
    exit(1);
  }
  cout << "Processing file." << endl;

  while(!inFS.eof()){
    if(lineCount = 0){
      inFS >> windowsOpen;
      office->setNumWindows(windowsOpen);
      ++lineCount;
    }else if(lineCount == nextClockTickLine){
      inFS >> clockTick;
      for(int i = 0; i < windowsOpen; ++i){
        office->checkTime(i, clockTick);//checking if each student is done
      }
      //here we need to check if all the students time at the window was satisfied
      //if the time wasnt satisfied we decrement the time m_timeNeeded
      //if it was satisfied we take them out of the window
      ++lineCount;
    }else if (lineCount == (nextClockTickLine + 1)){
      inFS >> numStudents;
      nextClockTickLine += numStudents + 2; // why is this + 2? im probably just dumb but im confused
      ++lineCount;
    }else{
      inFS >> studentTime;
      totalTime += studentTime;
      Student* s = new Student(studentTime, clockTick);
      queue->enqueue(s);//derefence it?
      // if(!office->isFull()){//office isnt full so we send the first student in line to a window
      //   for(int i = 0; i < windowsOpen; ++i){
      //     first = queue->dequeue(); // what if we move this part to a separate loop so first we queue all the students then start removing them
      //     office->assignWindow(first);
      //     // delete first;
      //   }
      // }
      delete s;
      ++lineCount;
    }
  }

  // could be a while(!queue->isEmpty() && office->checkOpen()) and run all code in there
  // checkOpen could return a boolean if all windoows are open (currently have this method only incrementing idleTime)
  time = 0;
  while(!queue->isEmpty() || !office->checkOpen()){//why are we doing check open? it will quit if any window is occupied
    if(!office->isFull() && !queue->isEmpty()){
      Student* first = queue->peek();
      if(first->getArrival() < time){
        continue;
      }else{
        first = queue->dequeue();
        office->assignWindow(first);
        students->insertBack(first);
      }
    }else if(!office->isFull() && queue->isEmpty()){
      office->checkOpen(); // increments idle time for open windows without a student
    }else{
      for(int i = 0; i < queue->getSize(); ++i){
        // student->incrementIdleTime(time);
        // ^^for each student in the queue call incrementIdleTime
        // need to be able to access each element of the queue
      }
    }
    time++;
  }

  // totalTime /= numWindows; this isn't right but we can do something to totalTime
  // while(totalTime > 0){ // something like this or a for loop with increasing clock tick
  //   totalTime--;
  // }

  // delete first;
  delete queue;
  delete office;
  delete students;
}

void Simulation::Calculate(){
  float totalStudentWait = 0.0;
  float waitTime = 0.0;
  int size = 0;

  size = students->getSize();

  for(int i = 0; i < size; ++i){
    Student* s = students->accessAtPos(i);
    waitTime = s->getWaitTime();
    totalStudentWait += waitTime;
    if(waitTime > m_longestStudentWait){
      m_longestStudentWait = waitTime;
    }
    if(i == (size/2)){
      m_medianStudentWait = waitTime;
    }
    if(waitTime > 10){
      m_studentsOverTen++;
    }

    delete s;
  }
  m_meanStudentWait = (totalStudentWait/size);

  // still need to calculate these
  m_meanWindowIdle;
  m_longestWindowIdle;
  m_windowsIdleOver5;

  cout << "Mean Student Wait: " << m_meanStudentWait << endl;
  cout << "Median Student Wait: " << m_medianStudentWait << endl;
  cout << "Longest Student Wait: " << m_longestStudentWait << endl;
  cout << "Students who waited over ten minutes: " << m_studentsOverTen << endl;
  cout << "Mean Window Idle Time: " << m_meanWindowIdle << endl;
  cout << "Longest Window Idle Time: " << m_longestWindowIdle << endl;
  cout << "Windows idle over five minutes: " << m_windowsIdleOver5 << endl;
}
