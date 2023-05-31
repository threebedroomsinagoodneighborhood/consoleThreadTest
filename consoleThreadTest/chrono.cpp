#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
//chrono
int chronomain(int argc,char * argv[]){
	chrono::seconds a(2);
	auto start = chrono::high_resolution_clock::now();//int start = clock(); // время старта
	this_thread::sleep_for(a);
	auto end = chrono::high_resolution_clock::now();//double time = (double)(end - start) / CLOCKS_PER_SEC;
	chrono::duration interval = end - start;
	cout<<start.time_since_epoch()<<"\n"
		<<interval<<", "<<interval.count();
	return 0;
}

//detach
#include <mutex>
#include <vector>
template <typename T>
class container{
	vector<T> _elements; mutex _lock;
public:
	container(){};
	void add(T t){ 
		lock_guard<mutex> guard(_lock);
		_elements.push_back(t); 
	}
	void print(){ for (auto e:_elements) cout<<e<<" "; }
};
void write(container<int> & c,int a){ c.add(a); }
int writemain(int argc,char * argv[]){
	container<int> ct;
	//thread t1(write,ref(ct),1);
	//thread t2(write,ref(ct),2);
	//thread t3(write,ref(ct),3);
	//t1.join(); t2.join(); t3.join();
	const int l=20; thread arr[l];
	for (int i=0; i<l; i++)
		arr[i]=thread(write,ref(ct),i);
	for (int i=0; i<l; i++)
		if (arr[i].joinable()) arr[i].join();
	ct.print();
	return 0;
}

//---------день 2: auto fs= 5s; считается как секунды, заменяет fs=chrono::seconds(5);
using namespace std::chrono_literals;
void run(int count){ while (count-- >0) cout<<count<<"\n"; this_thread::sleep_for(1s); }

int runmain(int argc,char * argv[]){
	thread t1(run,10);//,t2(run,5);
	//cout<<"start\n"; t1.join(); cout<<"end\n"; t2.join();//будет писать end последним
	
	if (t1.joinable()) t1.detach(); this_thread::sleep_for(5us);//detach обязательно с sleep_for чисто чтобы тред шел дольше чем мейн и успел доработать
	if (t1.joinable()) t1.join(); /*не joinит*/ cout<<"end\n";//то есть join это барьер, мейн всегда ждет тред. detach иначе, у них гонка
	return 0;
}
//---------задача?
void zapis(vector<int> vector,int n){ for (int i=0;i<n;i++) vector.push_back(i); }
void udalenie(vector<int> vector,int n){ while (n-->0) if (!vector.empty()) vector.pop_back(); }
void vyvod(vector<int> vector){ for (auto i: vector) cout<<i; }
int weirdmain(int argc,char * argv[]){
	vector<int> lol; thread t1(zapis,lol,10),t2(udalenie,lol,2),t3(vyvod,lol);
	t1.join();	t2.join();	t3.join();
	return 0;
}

vector<int> v; 
void T3(){ cout<<"\nchange: "; for (auto i:v) cout<<i<<" "; }
void T1(int n){ for (int i=0; i<n; i++) { v.push_back(i); T3(); } }
void T2(){ while (!v.empty()){ v.pop_back(); T3(); } }
int vTtmain(int argc,char * argv[]){	
	thread t1(T1,10),t2(T2),t3(T3);
	t1.join();	t2.join();	t3.join();
	return 0;
}

void add(int i){ v.push_back(i); }
void del(){ if (!v.empty()) v.pop_back(); }
void out(){ cout<<"\nchange: "; for (auto i:v) cout<<i<<" "; }
int badmain(){
	int const n=10; int i=n;
	thread arr1[n],arr2[n],arr3[n];
	while (!v.empty()) {
		while (i-->0){
			arr1[i]=thread(add,i);	arr2[i]=thread(del);	arr2[i]=thread(out);
			arr1[i].join(); arr2[i].join(); arr3[i].join();
		}
	}
	return 0;
}//не т_т