//def  Fibonacci(n) : 
//
//    p, q, r = 1, 1, 0
//    N = n//2; par = n%2
//    while N > 0:
//
//# Thread_1        Thread_2           Thread_3      Thread_4
//
//pp = p * p;       qq = q * q;          rr = r * r;     pr = p + r;       #  Òàêò 1
//p1 = pp + qq;   r1 = qq + rr;      q1 = q * pr;                      #  Òàêò 2
//p2 = p1 + q1;                                                      #  Òàêò 3
//
//if N % 2 == 1:   p, q, r = p2, p1, q1
//else:            p, q, r = p1, q1, r1
//N = N // 2
//
//if par == 0 : return r
//else : return q
//ïèòîíû êàêèå-òî
// 
//double Fibtemp(int n){
//	int p=1,q=1,r=0,N = n/2,par = n%2,
//		pp,p1,p2, qq,r1,q1, rr,pr;
//	while (N > 0){
//		pp=p*p; p1=pp+qq; p2=p1+q1;//thread1
//		qq=q*q; r1=qq+rr;//thread2
//		rr=r*r; q1=q*pr;//thread3
//		pr=p+r;//thread4
//
//		if (N%2==1) { p=p2; q=p1;  r=q1; }
//		else { p=p1; q=q1; r=r1; }
//		N/=2;
//	}
//	if (par==0) return r;
//	else return q;
//}//æóòü
//çäåñü äîëæíû áûòü êàêèå-òî yield?

//int Fibprep(int n){
//	int p=1,q=1,r=0,N = n/2,par = n%2,
//		pp,p1,p2,qq,r1,q1,rr,pr;
//	while (N > 0){
//		pp=p*p; qq=q*q; 
//		p1=pp+qq; pr=p+r; 
//		q1=q*pr; p2=p1+q1;
//		rr=r*r; r1=qq+rr;
//
//		if (N%2==1) { p=p2; q=p1;  r=q1; }
//		else { p=p1; q=q1; r=r1; }
//		N/=2;
//	}
//	if (par==0) return r;
//	else return q;
//}

#include <thread>
#include <iostream>
#include <mutex>
#include <latch>
#include <windows.h>
#include <cstdlib>
using namespace std;

int const n = 20;
int p=1,q=1,r=0,N=n/2,par=n%2,
		pp,p1,p2, qq,r1,q1, rr,pr;

//mutex t1,t2; 
latch t1(4),t2(3);

void Fibthread1(int n){
	pp=p*p;		//áàğüåğ 1 
	t1.arrive_and_wait();
	p1=pp+qq;	//áàğüåğ 2
	t2.count_down();
	t2.wait();
	p2=p1+q1;	//îêîí÷àíèå
}
void Fibthread2(int n){
	qq=q*q;		//áàğüåğ 1
	t1.arrive_and_wait();
	r1=qq+rr;	//áàğüåğ 2
	t2.count_down();
}
void Fibthread3(int n){
	rr=r*r;		//áàğüåğ 1
	t1.arrive_and_wait();
	q1=q*pr;	//áàğüåğ 2
	t2.count_down();
}
void Fibthread4(int n){
	pr = p + r;	//áàğüåğ 1
	t1.count_down();
}
void FibchangeN(int n){
	if (N%2==1) { p=p2; q=p1;  r=q1; }
	else { p=p1; q=q1; r=r1; }
	N/=2;
}
void Fibreturn(int n){
	if (par==0) cout<< r;
	else cout<< q;
}
int Fibc(int n){
	if (n==0) return 0;
	if (n==1) return 1;
	return Fibc(n-2)+Fibc(n-1);
}

int fibmain(int argc,char * argv[]){
	int fib=0;
	int start = clock(); // âğåìÿ ñòàğòà
	thread 
		t1(Fibthread1,n),
		t2(Fibthread2,n),
		t3(Fibthread3,n),
		t4(Fibthread4,n)//,
		//tN(FibchangeN,n),
		//tR(Fibreturn, n)
		;
	while (N>0){
		if (t1.joinable())	t1.join();
		if (t2.joinable())	t2.join();
		if (t3.joinable())	t3.join();
		if (t4.joinable())	t4.join(); 
		//òàê?
		//t1.detach();
		//t2.detach();
		//t3.detach();
		//t4.detach();
		cout<<"\npp="<<pp<<", p1="<<p1<<", p2="<<p2<<", qq="<<qq<<", r1="<<r1<<", q1="<<q1<<", rr="<<rr<<", pr="<<pr;
		//if (tN.joinable())	tN.join();	  else 
		FibchangeN(n); cout<<endl<<N;
	}
	int end = clock(); // âğåìÿ îêîí÷àíèÿ
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout<<"\nthread time: "<<time<<endl;
	//Fibreturn(n);
	//if (tR.joinable())	tR.join(); cout<<endl;
	for (int i=1; i<n; i++){
		if (Fibc(i)!=fib)cout<<"   !";
		cout<<"\t"<<i<<": "<<Fibc(i)<<"  "<<fib<<endl;
	}
	//int fib[n]={0};
	//fib[0]=1; fib[1]=1;
	//for (int i=2; i<n; i++) fib[i]=fib[i-1]+fib[i-2];
	//for (int i=0; i<n; i++) cout<<"\n"<<fib[i];
	return 0;
}