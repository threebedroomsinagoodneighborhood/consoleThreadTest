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
/*#include <mutex>
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
}*/

//------------------------день третий, повтор, лучше каждую стадию закоммитить
/*#include <mutex>
#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;
mutex m; thread::id mainid=this_thread::get_id();
void c(){ thread::id id=this_thread::get_id(); if (mainid==id) cout<<"main\n"; else cout<<"not main\n"; }
void p(int n){ m.lock();  cout<<n<<": "; c(); cout<<this_thread::get_id()<<"\n"; m.unlock(); }//lockguard<mutex> g(m);
int prevmain(int argc,char * argv[]){
    cout<<mainid<<"\n"; c();
    thread t1(p,1); thread t2(p,2); thread t3(p,3); thread t4(p,4);
    t1.join();             t2.join();      t3.join();      t4.join();
    return 0;
}
//работа с массивами
void sum(int n,int arr[],int index){
    thread::id id=this_thread::get_id();
    cout<<"("<<id<<" begin: ";
    int s=0; for (int i=0; i<n; i++) s+=i;
    arr[index]=s;
    cout<<id<<" finish;) ";
}    
int arrmain(int argc,char * argv[]){
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
        }//mutex лишний для вычислительных программ, но в результате для текста внутри идет перемешка
    return 0;
}
*/

//------------------------задачи
#include <mutex>
#include <iostream>
#include <thread>
#include <windows.h>
#include <cstdlib>
using namespace std;
double pml(int n){//рекуррентная формула мадхава-лейбница
    if (n==0) return 1;//почему-то без этой строки все плохо
    while (n>0) return pow(-1,n)/(2*n+1) + pml(n-1); 
}
void pmlsep(int n,double arr[]){ arr[n]=pow(-1,n)/(2*n+1);}//arr[index]=a; return a; }
int pmlmain(int argc,char * argv[]){
    //int hc=thread::hardware_concurrency(); cout<<hc;//ЕХЕХЕ А У МЕНЯ ВОСЕМЬ
    //cout<<pml(800)*4;//чтобы потоки были полезны лучше всего массив делать
    int start = clock(); // время старта
    int const N=100;
    double arr[N]={0},pi=0; thread arrt[N];
    for (int i=0; i<N; i++) arrt[i]=thread(pmlsep,i,arr);
    for (int i=0; i<N; i++)
        if (arrt[i].joinable()){
            arrt[i].join(); pi+=arr[i]; //cout<<arr[i]<<endl;
        }
    cout<<pi*4;
	int end = clock(); // время окончания
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout<<endl<<time;
    return 0;
}
//формула валлиса
double pv(int n){
    if (n==0) return 1;
    while (n>0) return 4*pow(n,2)/(4*pow(n,2)-1)*pv(n-1);
}
void pvsep(int n,double arr[]){
    if (n==0) arr[n]=1;//надо умножать с 1 а не с 0 поэтому if нужно
    else arr[n]=4*pow(n,2)/(4*pow(n,2)-1); 
}
int pvmain(int argc,char * argv[]){
    //cout<<pv(1000)*2;
    int start = clock(); // время старта
    int const N=100;
    double arr[N]={0},pi=1; thread arrt[N];//вместо p=0 -> p=1
    for (int i=0; i<N; i++) arrt[i]=thread(pvsep,i,arr);
    for (int i=0; i<N; i++)
        if (arrt[i].joinable()){
            arrt[i].join(); pi*=arr[i];//вместо += -> *=
        }
    cout<<pi*2;//вместо *4 -> *2
    int end = clock(); // время окончания
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    cout<<"\nсекунд на подсчет: "<<time;
    return 0;//наверное я зря новый мейн писала, можно было новый цикл for в предыдущем создать для новой формулы
}
//интрегрирование... диос мио...
//ехе по опечаткам сразу видно что иванюхин задавал))
//пока только пи проверяю на точность
int const N=100;//когда все поменяла на long double что-то сломалось?
double const realpi=3.1415926535897932384626433832795028841971693993751058209749;//68 вроде цифер причем не все вмещаются похоже без long
double const E=0.001;//точность
void compare(double pi,bool f){ if (fabs(realpi-pi<E)) f=1;}//я не умею пока что сравнивать нормально
int taskmain(int argc,char * argv[]){
    double arrml[N]={0},arrv[N]={0},
           piml=0,      piv=1;
    thread arrtml[N],   arrtv[N],
        comparepi[N];
    bool enough=0;
    for (int i=0; i<N; i++) { 
        arrtml[i]=thread(pmlsep,i,arrml),arrtv[i]=thread(pvsep,i,arrv);
    }
    //while (thread comppi(compare,piml)){//в поток надо пихнуть функцию сравнения точности..
    for (int i=0; i<N; i++)
        if (arrtml[i].joinable()){
            arrtml[i].join(); piml+=arrml[i]; 
            //comparepi[i]=thread(compare,piml*4,enough);//это же последовательно будет? после каждого пи останавливать программу чтобы проверить но проверка внутри треде? здесь нет параллельности
            //if (comparepi[i].joinable()) comparepi[i].join();//только если каждый результат пи записывать в еще один массив и проверять все результаты массива в треде ожидая что программа не пройдет весь массив, и тогда мы зря считали более точные пи без остановки
            cout<<piml*4<<", ";
            compare(piml*4,enough);
            if (enough) { cout<<"\nstop\n"; break; }
        }
    //cout<<"\n"<<piml*4<<"\n\n";
    printf("\n%.50f\n\n",piml*4);
    enough=0;//чтобы вместить обе формулы в один цикл надо два флага создать, пока обойдусь одним и поменяю его обратно на 0 в конце первого цикла
    for (int i=0; i<N; i++)
        if (arrtv[i].joinable()){
            arrtv[i].join(); piv*=arrv[i];
            cout<<piv*2<<", ";
            compare(piv*2,enough);
            if (enough) { cout<<"\nstop\n"; break; }
        }
    //cout<<"\n"<<piv*2;
    printf("\n%.50f\n\npi:\n%.50f",piv*2,realpi);
    return 0;//здесь у меня не работает сравнение на точность. хелп
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