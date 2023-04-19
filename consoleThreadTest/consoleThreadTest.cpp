// consoleThreadTest.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;

//void hi(){ cout<<"hi\n"; }
//void bye(){ cout<<"bye\n"; }
void a(){ Sleep(500); cout<<"\nthread in func: "<<this_thread::get_id(); Sleep(500); }

int main(){
    thread A(a);
    cout << "threads:\n\n";
    cout<<"thread in main: "<<A.get_id();
    Sleep(500); A.join();
    cout<<"\n\nexe complete\n";
}
//при свертывании строки join во время работы вызывается abort и в консоль сообщение треда выводится в самом конце после строки exe complete

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file