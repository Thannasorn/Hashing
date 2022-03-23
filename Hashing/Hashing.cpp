#include<iomanip>
#include<iostream>
#include<string>
#include<vector>

using namespace std;
int nextPrime(int size);

template <class HashedObj>
class HashTable
{
public :
	explicit HashTable(const HashedObj & notFound, int size=101);
	HashTable(const HashTable & rhs): ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), currentSize(rhs.currentSize) {}

	const HashedObj & find(const HashedObj & x) const;
	void makeEmpty();
	void insert(const HashedObj & x);
	void remove(const HashedObj & x);
	void print(); 
	void create(int size);
	const HashedObj & findarray(const HashedObj & x) const;

	const HashTable & operator=(const HashTable & rhs);

	enum EntryType { ACTIVE, EMPTY, DELETED };
	int isprime(int x)const;
private :
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry( const HashedObj & e = HashedObj( ),EntryType i = EMPTY ) : element(e),info(i) {}
	};

	vector<HashEntry> array;
	int currentSize;
	const HashedObj ITEM_NOT_FOUND;

	
	bool isActive(int currentPos) const;
	int findPos(const HashedObj & x) const;
	void rehash();

	int nextPrime(int size);
	int hash( const string & key, int tableSize ) const;
    int hash( int key, int tableSize ) const;
	void print(int tableSize) const; 
	int  hash2(int x,int tablesize)const;
	
};
////////////////////////////////////////create////////////////////////////////////////////////////////
template <class HashedObj>
void HashTable<HashedObj>::create(int size)
{
	array.resize(size);
	makeEmpty();
}


////////////////////////////////////////print///////////////////////////////////////////////////////////////

template <class HashedObj>
void HashTable<HashedObj>::print() 
{
	
	print(array.size());

}

template <class HashedObj>
void HashTable<HashedObj>::print(int tableSize) const
{
	cout << "\t\t\t------------------------\n\t\t\t";
	cout << setw(6);
	cout << "[ADDR][";
	cout << setw(7);
	cout << "ELEMENT][";
	cout << setw(9);
	cout << "INFO]\n";
	for (int i = 0; i<tableSize; i++)
	{
		string info = array[i].info == 0 ? "ACTIVE" : array[i].info == 1 ? "EMPTY" : "DELETED";
		cout << "\t\t\t------------------------\n\t\t\t";
		cout << "[" << setw(4) << i << "][" << setw(7);
		cout << array[i].element;
		cout << "][";
		cout << setw(7);
		cout << info;
		cout << "]\n";
	}
	cout << "\t\t\t------------------------\n";
}

template <class HashedObj>
HashTable<HashedObj>::HashTable( const HashedObj & notFound , int size) : ITEM_NOT_FOUND(notFound),array(nextPrime(size))
{
makeEmpty();
}

/////////////////////makeEmpty///////////////////////////

template<class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	currentSize= 0 ;
	for(int i=0;i<array.size();i++)
		array[i].info= EMPTY;

}

//////////////////////find////////////////////////////

template<class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x ) const
{
	int currentPos = findPos(x);
	return isActive(currentPos) ? array[currentPos].element : ITEM_NOT_FOUND;
}
//////////////////////findarray////////////////////////////

template<class HashedObj>
const HashedObj & HashTable<HashedObj>::findarray(const HashedObj & x ) const
{
	int currentPos = findPos(x);
	int index = isActive(currentPos) ? currentPos : ITEM_NOT_FOUND;
	if(index!=ITEM_NOT_FOUND) cout<<"\n\t!!! ADDRESS OF "<<x<<" IN ARRAY INDEX IS "<<index<<" !!!\n\t!!! AND ADDRESS OF MEMORY IS "<<&array[index]<<" !!!\n\n";
	return index;
}

//////////////////////findPos////////////////////////////

template<class HashedObj>
int HashTable<HashedObj> ::findPos(const HashedObj & x ) const
{
	int collisionNum=0;
	int currentPos = (hash(x, array.size()) + 0) % array.size(); //open adressing

	while(array[currentPos].info !=EMPTY&&array[currentPos].element != x )
	{

		currentPos = hash(x,array.size())+(++collisionNum)*hash2(x,array.size()); //double hashing
		
		if(currentPos>=array.size())
			currentPos = currentPos%array.size();
	}
	return currentPos;
}
template<class HashedObj>
int HashTable<HashedObj> ::hash2(int x,int tablesize) const
{
	int prime=isprime(tablesize);
	return (prime-(x%prime));
}

template<class HashedObj>
int HashTable<HashedObj> ::isprime(int x) const 
{
	int prime=0,check=0;
	for(int i=0;i<x;i++)
	{
		check=0;
		for(int j=1;j<=i;j++)
		{
			if(i%j==0)
			{
				check++;
			}
		}
		if(check<3)
			{
				prime=i;
			}
	}
	return prime;
}

/////////////////////isActive///////////////////////////

template<class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
	return array[currentPos].info==ACTIVE;
}

////////////////////insert//////////////////////////////
 
template<class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{
	int currentPos=findPos(x);
	if(isActive(currentPos))
		return;
	array[currentPos]=HashEntry(x,ACTIVE);

	if(++currentSize>array.size()*0.70)
		rehash();

}

/////////////////////remove////////////////////////////

template<class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x)
{
	int currentPos=findPos(x);
	if(isActive(currentPos))
		array[currentPos].info=DELETED;
}

///////////////////rehash////////////////////////////

template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
	vector<HashEntry> oldArray=array;

	array.resize(nextPrime(2*oldArray.size()));
	for(int j=0;j<array.size();j++)
		array[j].info=EMPTY;

	currentSize=0;
	for(int i=0;i<oldArray.size();i++)
		if(oldArray[i].info==ACTIVE)
			insert(oldArray[i].element);
}

//////////////////////nextPrime///////////////////////////

template <class HashedObj>
int HashTable<HashedObj>::nextPrime(int size)
{
	int i=2;
	
	while(size%i!=0) i++;
		if(i==size) return i;
		else{
			i=size+1;
			bool check=false;
				while(true){
					for(int j=2;j<=i;j++){
						if(i%j==0&&j<i) break;
						else if(i%j==0&&j==i) check = true; 
					}
				if(check) break;
				i++;
				}
			return i;
		}
}


/////////////////////hash////////////////////////////

template<class HashedObj>
int HashTable<HashedObj>::hash(const string & key,int tableSize) const
{
	int hashVal = 0;
	for(int i=0;i<key.length();i++)
		hashVal+=key[i];

	return hashVal % tableSize;

}

template<class HashedObj>
int HashTable<HashedObj>::hash(int key ,int tableSize) const
{
	if( key < 0 ) key -= key;
    return key % tableSize;
}

void main(){
	HashTable<int> a(-999);
	string choice;
	bool exit = true;
	bool create =false;

	do{
	system("cls");
	cout<<"\t====== Hashing ======\n\n";
	cout<<"\t1.) Create hash table\n\t2.) Insert\n\t3.) Print\n\t4.) Find\n\t5.) Remove\n\t6.) Exit\n\n\tEnter Choice : ";
	cin>>choice;
	switch (choice[0])
	{
		int x;
////////////////////////////////////CreateHashTable////////////////////////////////////////////////////////////////////////
		case '1' :
			system("cls");
			if(!create){
				int cell;
				cout<<"\t====== Create hash table ======\n\n\tEnter size of hash table : ";
				cin>>cell;
				if(cell>29){
					cout<<"\n\t!!!Table Size Value Over 30 Try Again!!!\n\n";
					system("pause");
				}else{
					cout<<"\n\tNext prime of "<<cell<<" is "<<nextPrime(cell)<<"\n\tSize of hash table is "<<nextPrime(cell)<<endl<<endl;
				
					a.create(nextPrime(cell));
					create =!create;

					system("pause");
				}
			}
			else 
			{
				cout<<"\n!!!CREATED ALL MODE AVAILABLE!!!\n\n";
				system("pause");
			}
				
			break;
////////////////////////////////////INSERT////////////////////////////////////////////////////////////////////////
		case '2' :
			system("cls");
			if(create)
			{
			cout<<"\t====== Insert ======\n\n\tEnter element to Insert : ";
			cin>>x;
			if(a.find(x)==-999){
				a.insert(x);
				cout<<"\n\t!!! INSERT ELEMENT SUCCESSFULL !!!\n";
			}
			else cout<<"\n\t!!! Hash table have same element please try again !!!\n";
			cout<<endl;
			system("pause");
			}
			else 
			{
				cout<<"\n!!!MUST CREATE TABLE BEFORE INSERT ELEMENT!!!\n\n";
				system("pause");
			}
			break;
////////////////////////////////////PRINT////////////////////////////////////////////////////////////////////////
		case '3' :
			system("cls");
			if(create)
			{
				cout<<"\t====== Print ======\n\n";
				a.print();
				cout<<endl;
				system("pause");
			}
			else 
			{
				cout<<"\n!!!MUST CREATE TABLE BEFORE PRINT HASH TABLE!!!\n\n";
				system("pause");
			}
			break;

////////////////////////////////////FIND////////////////////////////////////////////////////////////////////////
		case '4' :
			system("cls");
			if(create)
			{
				cout<<"\t====== Find ======\n\n\tEnter Element to Find : ";
				cin>>x;
				if(a.findarray(x)==-999)
					cout<<"\n\t!!! FIND ELEMENT NOT FOUND !!!\n";
				cout<<endl;
				system("pause");
			}
			else 
			{
				cout<<"\n!!!MUST CREATE TABLE BEFORE FIND ELEMENT!!!\n\n";
				system("pause");
			}
			break;
////////////////////////////////////REMOVE////////////////////////////////////////////////////////////////////////
		case '5' :
			system("cls");
			if(create)
			{
			cout<<"\t====== Remove ======\n\n\tEnter Element to Remove : ";
			cin>>x;
			if(a.find(x)!=-999){
				a.remove(x);
				cout<<"\n\t!!! REMOVE ELEMENT SUCCESSFULL !!!\n";
			}
			else cout<<"\n\t!!! HASH TABLE HAVEN'T ELEMEMT PLEASE TRY AGAIN !!!\n";
			cout<<endl;
			system("pause");
			}
			else 
			{
				cout<<"\n!!!MUST CREATE TABLE BEFORE REMOVE ELEMENT!!!\n\n";
				system("pause");
			}
			break;

		case '6' : 
			exit = false;
			break;
		default : break;
	}
	}while(exit);

}


int nextPrime(int size)
{
	int i=2;
	if (size==1) return 1;
	while(size%i!=0) i++;

		if(i==size) return i;
		else{
			i=size+1;
			bool check=false;
				while(true){
					for(int j=2;j<=i;j++){
						if(i%j==0&&j<i) break;
						else if(i%j==0&&j==i) check = true; 
					}
				if(check) break;
				i++;
				}
			return i;
		}
}