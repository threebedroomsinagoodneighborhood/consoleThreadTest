//12:00
//вместо изменения изначального кода я ставлю время и новые линии кода 
//сверху project -> properties -> language -> openmp support - yes/no
#include <iostream>
using namespace std;
int checkmain(int argc,char * argv[]){
#ifdef _OPENMP
	cout<<"1\n";//openmp.org/
#endif
	return 0;
}
//12:13
#include <omp.h>
int wowmain(int argc,char * argv[]){//вот это игры...
	int c=0;
#pragma omp parallel//по дефолту создаются 4? потока если не вставить {} после прагмы
	{				//судя по всему для меня это 8
		c++;		//один запуск дал 328?!
//#pragma omp critical
		cout<<c<<"\nhuh 12:13\n"; //после добавки critical все значения = 8 кроме первого
	}				//ааа у меня максимум = 8 и двухзначные трехзначные значения это только смесь cout
//12:21
	omp_set_num_threads(5);
#pragma omp parallel reduction(+:c)
	{
//#pragma omp critical
		cout<<c<<"count; "<<omp_get_num_threads()<<"threads 12:21\n";
	}
//12:30
	omp_set_num_threads(2);
#pragma omp parallel shared(c)
	{
		cout<<c<<"12:30\n";
	}
//12:33
	cout<<"\n\tfinish";
	return 0;
}
//12:40 ========================================================== задача
int funcsum(int a[],int b[],int N){//суммировать значения массива но с омп
	int sum=0;
#pragma omp parallel for reduction(+:sum)
	for (int i=0; i<N; i++)//без for дает сумму 256 (32*8)
		sum+=a[i]*b[i];
	//cout<<"\t"<<omp_get_num_threads();
	return sum;
}
int funcmain(int argc,char * argv[]){
	const int N=3;//4+10+18=32
	int a[N]={1,2,3},b[N]={4,5,6}; cout<<"\t"<<funcsum(a,b,N);
	return 0;
}
//12:50 ------------ версия 2 на статический двумерный массив
const int N=3;
int c[N];
void f(int a[],int b[3Ui64][3Ui64],int N){ //я забыла про поинтеры поэтому я вставила те буковки которые код выводит если мышкой навести на массив b
	int sum=0; 
#pragma omp parallel for reduction(+:sum)
	for (int i=0; i<N; i++){
		c[i]=b[i][0];		 for (int j=1; j<N; j++) c[i]+=b[i][j];
	}
}
int staticmain(int argc,char * argv[]){
	//srand(time(NULL));
	int a[N], b[N][N];
	for (int i=0; i<N; i++){
		a[i]=rand()%10;		 for (int j=0;j<N;j++) b[i][j]=rand()%10;
	}
	//cout<<"a:\n";			 for (int i=0; i<N; i++) cout<<a[i]<<"\n";
	cout<<"b:\n";
	for (int i=0; i<N; i++)	{for (int j=0; j<N; j++) cout<<b[i][j]<<" "; cout<<"\n";}
	f(a,b,N); cout<<"c:\n";	 for (int i=0; i<N; i++) cout<<c[i]<<"\n";
	return 0;
}
//13:16 переделать под динамический массив с рандомной длиной b[][j]
//16:43 ------------ версия 3
int const row=5;
void fn(int a[], int*b[], int c[]){
#pragma omp parallel for
	for (int i=0; i<row; i++) for (int j=0; j<c[i]; j++) a[i]+=b[i][j];
}
int dynamicmain(int argc,char * argv[]){
	//srand(time(NULL));
	int a[row]={0},*b[row],column[row];
	for (int i=0; i<row; i++){
		column[i]=rand()%10+2;	b[i]=new int[column[i]];
		for (int j=0; j<column[i]; j++) b[i][j]=rand()%10;
	} fn(a,b,column);
	for (int i=0; i<row; i++){ for (int j=0; j<column[i]; j++)	
		cout<<b[i][j]<<" "; cout<<"= "<<a[i]<<"\n"; }
	return 0;
}//17:00 ==========================================================
//17:30
void r(int level){
#pragma omp single//выполняется только одним тредом после чего остальные треды не делают это
	{ cout<<"level "<<level<<": number of threads: "<<omp_get_num_threads()<<"\n"; }
}
int main(int argc,char * argv[]){
#pragma omp parallel num_threads(2)
	{ r(1);
#pragma omp parallel num_threads(2)
		{ r(2);
#pragma omp parallel num_threads(2)
			{ r(3); } } }
	return 0;
}