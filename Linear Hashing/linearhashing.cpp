#include <bits/stdc++.h>
#define SZ(a) (int)(a.size())
#define rep(i,l,h) for(int i=(l); i<=(h);i++)
using namespace std;

vector<vector<int> > hash_table;
vector<int> counter; // counter[i] points to the index in ith bucket to place a new number
int N, splitPointer, B;

void initHashTable(){
	vector<int> v;
	v.clear();
	N = 3;
	splitPointer=0;
	rep(i, 0, N-1){
		hash_table.push_back(v);
		counter.push_back(0);
	}
}

void insertRecord(int a){
	vector<int> v;
	v.clear();
	/* calculate hash value*/
	int cnt, b = a%N;
	if( b<0){
		b+=N;
	}
	if(b<splitPointer){
		b = a%(2*N);
		if(b<0){
			b+=2*N;
		}
	}

	/*insert hash value in hash table*/
	if(counter[b]>=SZ(hash_table[b]))
		hash_table[b].push_back(a);
	else
		hash_table[b][counter[b]]=a;
	counter[b]++;

	/*Overflow*/
	if(counter[b]>B){

		/* split the bucket pointed to by split pointer */
		cnt = counter[splitPointer]-1;
		counter[splitPointer]=0;

		rep(i, 0, cnt){
			a = hash_table[splitPointer][i];
			b = a%(2*N);
			if(b<0){
				b+=2*N;
			}

			if(b!=splitPointer){
				/*make space for b buckets*/
				rep(j, SZ(hash_table), b){
					counter.push_back(0);
					hash_table.push_back(v);
				}
			}

			/*insert hash value in hash table*/
			if(counter[b]>=SZ(hash_table[b]))
				hash_table[b].push_back(a);
			else
				hash_table[b][counter[b]]=a;
			counter[b]++;

		}
		splitPointer++;
		if(splitPointer==N){
			splitPointer=0;
			N*=2;
		}
	}
}

bool search_linearhash(int a){
	int b = a%N;
	if(b<0){
		b+=N;
	}

	if(b<splitPointer){
		b = a%(2*N);
		if(b<0){
			b+=2*N;
		}
	}

	rep(i, 0, counter[b]-1){
		if(hash_table[b][i] == a)
			return false;
	}
	return true;
}

int main(int argc, char const *argv[])
{
	int ch;
	int M;
	if(argc != 4)
		cout<<"Insufficient input arguments\n";
	else{
		int a, val, fl=0;
		string line;
		M = atoi(argv[2]);
		B = atoi(argv[3])/4;  // Bucket capacity
		FILE *p;
  	p = fopen(argv[1], "r");
		initHashTable();

 //input from file
  	while(fscanf(p, "%d", &ch)!=EOF){
  		if(search_linearhash(ch)) {
    		cout <<ch  << endl;
    		insertRecord(ch);
   			//printHashTable();
   		}
  		//else
  			//cout << "Element is already present in hash table" << endl;
  	}
	}
	return 0;
}
