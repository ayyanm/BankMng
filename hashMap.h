#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <unordered_map>


using namespace std;

template <class t1, class t2>
class hashMap
{
public:
	hashMap(std::size_t = 5);
	t2& operator[](t1);
private:
	struct keyVal
	{
		keyVal()
		{
			key = t1();
			value = t2();
		}

		t1 key;
		t2 value;
	};

	void resize(std::size_t);

	std::size_t hash1(std::string);
	std::size_t hash2(std::string);

	std::vector<keyVal> table1;
	std::vector<keyVal> table2;

	std::size_t items1;
	std::size_t items2;
};



/*
 * constructor for hashmap class
 *
 * takes in size 
 * sets default vals
 */
template <class t1, class t2>
hashMap<t1, t2>::hashMap(std::size_t size)
{
	items1 = 0;
	items2 = 0; 

	table1.resize(size);
	table2.resize(size);
}



/*
 * []operator 
 * find and search is implemented here, 
 * calls resize if load factor too high
 * param is key 
 * returns pointer to value
 */
template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
{
	// checking load factor
	double point2 = .2; 
	int loadLim1 = table1.size() * point2; //gets 20% of size
	int loadLim2 = table2.size() * point2;
	bool collision = false; 
	//if load factor is over 20%, resize * 2
	if (items1 >= loadLim1)
	{
		this->resize(table1.size() * 2);
	} 

	if (items2 >= loadLim1)
	{
		this->resize(table1.size() * 2);
	}
	//getting indexes for checking slot
	std::size_t index1 = hash1(key) % table1.size();
	std::size_t index2 = hash2(key) % table2.size();
	
	//collision counters
	int i1 = 0;
	int i2 = 0; 

	do
	{
		// first check is if slot is empty
		if (table1[index1].key == t1())
		{
			//if its empty, put it in the table, increment items by 1
			table1[index1].key = key;
			items1++;
			return table1[index1].value;

		//next check if to see if element is found at that index already
		} else if (table1[index1].key == key)
		{
			return table1[index1].value; 
		//now same checks but for table 2 
		} else if (table2[index2].key == t1())
		{
			//if its empty, put it in the table, increment items by 1
			table2[index2].key = key;
			items2++;
			return table2[index2].value;

		//next check if to see if element is found at that index already
		} else if (table2[index2].key == key)
		{
			return table2[index2].value; 
		// at this point if it hasnt returned it means there has been a collision or nothing found 
		} else
		{
			i1++; 
			i2++; 
			collision = true; 

			//if its even 
			if (i1 % 2 == 0)
			{
				index1 = ( index1 + i1 * hash2 ( key )) % table1.size (); // on even iterations
			} else if (i1 % 2 != 0)
			{
				index1 = ( index1 + i1 * hash1 ( key )) % table1.size (); // on odd iterations
			}

			// same thing but for table2 		
			if (i2 % 2 == 0)
			{
				index2 = ( index2 + i2 * hash1 (key)) % table2.size (); // on even iterations
			} else if (i2 % 2 != 0)
			{
				index2 = ( index2 + i2 * hash2 (key)) % table2.size (); // on odd iterations
			}
		}
		//loop back if collision is true.  
	} while (collision == true);
}




/*
 * resize function
 *
 * takes in resize amount 
 * copies tables and resizes
 */
template <class t1, class t2>
void hashMap<t1, t2>::resize(std::size_t amount)
{	
	//setting up empty vectors
	std::vector<keyVal> copyTable1 = table1;
	std::vector<keyVal> copyTable2 = table2;
	size_t copyItem1 = items1;
	size_t copyItem2 = items2;

	//clears
	table1.clear();
	table2.clear();

	//resizes
	table1.resize(amount);
	table2.resize(amount);

	//copying to new
	for (size_t i = 0; i < copyTable1.size(); i++)
	{
		//if not empty
		if (copyTable1[i].key != t1())
		{
			(*this)[copyTable1[i].key] = copyTable1[i].value;
		}
	}
	
	//for table 2
	for (size_t i = 0; i < copyTable1.size(); i++)
	{
		//if not empty
		if (copyTable2[i].key != t1())
		{
			(*this)[copyTable2[i].key] = copyTable2[i].value;
		}
	}

}






/*
 * hash function
 *
 * summation of key[i] * 10^i 
 * 
 */
template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash1(std::string key)
{
	size_t k = key.length(); // saving length of string to k
	size_t result = 0; 
	for (int i = 0; i < k-1; i++)
	{
		result = result + key[i] * pow(10, i); 
	}
	return result; 
	
}





/*
 * hash function
 *
 * summation of key[i] * 10^i in reverse 
 * 
 */
template <class t1, class t2>
std::size_t hashMap<t1, t2>::hash2(std::string key)
{
	size_t k = key.length() - 1; // saving length of string to k
	size_t result = 0; 
	for (int i = k; i > 0; i--)
	{
		result = result + key[i] * pow(10, i); 
	}
	return result; 
}
