//------------------------день первый
/*
#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;

void a(){ Sleep(500); cout<<"\nthread in func: "<<this_thread::get_id(); Sleep(500); }

int main(){
    thread A(a);
    cout<<"threads:\n\n"
        <<"thread in main: "<<A.get_id();
    Sleep(500); A.join();
    cout<<"\n\nexe complete\n";
}

//при свертывании строки join во время работы вызывается abort и в консоль сообщение треда выводится в самом конце после строки exe complete
*/

//------------------------день второй
#include <mutex>
#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;
//на выходе программа должна завершаться в едином потоке. без join оказывается несколько потоков и вызывается ошибка тк невозможно выпринтить и thread и main без их соединения в общий поток
mutex a;   thread::id mainid=this_thread::get_id();
void is(){ thread::id i=this_thread::get_id(); if (mainid==i) cout<<i<<" is main\n"; else cout<<i<<" is not main\n"; }
void p(int x){ a.lock(); cout<<"\nthread "<<x<<"\n"; is(); a.unlock(); }
void f(int x){ a.lock(); cout<<"\nthread "<<x<<" id "<<this_thread::get_id()<<"\n"; is(); a.unlock(); }
//учимся мютексу
int wasmain(int argc, char* argv[]){//до работы с массивами
    thread t1(f,1),t2(f,2),t3(f,3); 
    thread::id b=this_thread::get_id();
    cout<<"\nmain id "<<b<<"\n"; is();
    t1.join(); t2.join(); t3.join();
    return 0;
}
//наилучшее применение тредов не в запутанном выводе текста а в ускоренных численных расчетах
//работа с массивами
void sum(int n,int arr[],int index){
    thread::id id=this_thread::get_id();
    cout<<id<<" begin:\n";
    int s=0; for (int i=0; i<n; i++) s+=i;
    arr[index]=s;
}
int main(int argc,char * argv[]){
    const int length=20;
    thread::id id;
    thread thread_array[length];
    int res_arr[length]={0};
    for (int i=0; i<length; i++) thread_array[i]=thread(sum,rand(),res_arr,i);
    for (int i=0; i<length; i++)
        if (thread_array[i].joinable()){
            id=thread_array[i].get_id();
            thread_array[i].join();
            cout<<"thread id "<<id<<"\tcalculated sum "<<res_arr[i]<<"\n";
        }
    //у меня смешения cout нету никакого а у препода есть..
    return 0;
}


//инструкции
/*// consoleThreadTest.cpp : This file contains the 'main' function. Program execution begins and ends there.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
*/