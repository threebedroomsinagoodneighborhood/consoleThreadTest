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
int main(int argc,char * argv[]){
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