
#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;

//Best Version
class GridWorld {

private:
	struct node
	{
		int id;
		node *next;
		node *prev;
	};
	struct person
	{
		int row;
		int col;
		bool alive;
		node *place;
	};
	struct district
	{
		int dPop;
		node* dfront;
		node* dback;
	};
	
	district **dArr;
	unsigned int idNum;
	unsigned int wPop;
	int wCol;
	int wRow;
	vector<person> pVector;
	district deadPool;
public:
	int val;
	
	/**
	* constructor:  initializes a "world" with nrows and
	*    ncols (nrows*ncols districtcs) in which all
	*    districtricts are empty (a wasteland!).
	*/
	GridWorld(unsigned nrows, unsigned ncols) 
	{
		idNum = 0;
		wPop = 0;
		wCol = ncols;
		wRow = nrows;
		dArr = new district*[nrows];
		for (int i = 0; i < nrows; i++)
		{
			dArr[i] = new district [ncols];
		}
		for (int i = 0; i < nrows; ++i) {
			for (int j = 0; j < ncols; ++j) {
				dArr[i][j].dPop = 0;
				dArr[i][j].dfront = nullptr;
				dArr[i][j].dback = nullptr;
			}
		}
		deadPool.dfront = nullptr;
		deadPool.dback = nullptr;
		// your constructor code here!
	}
	~GridWorld() {
		node *tmp ;	

		for (node *header = deadPool.dfront; header != nullptr; header = header->next)
		{
			tmp = header;
			delete tmp;
		}
		
		for (int i = 0; i < wRow; i++)
			delete[] dArr[i];
		delete[] dArr;


		// your destructor code here.
	}

	void push_back(node * tmp, int row, int col, int id)
	{
		
		if (dArr[row][col].dfront == nullptr)
			dArr[row][col].dfront = dArr[row][col].dback = tmp;
		else
		{
			node *remind;
			dArr[row][col].dback->next = tmp;
			remind = dArr[row][col].dback;
			dArr[row][col].dback=dArr[row][col].dback->next; 
			dArr[row][col].dback->prev= remind;
			dArr[row][col].dback->next = nullptr;
		}
		dArr[row][col].dPop++;
	}

	
	/*
	* function: birth
	* description:  if row/col is valid, a new person is created
	*   with an ID according to rules in handout.  New person is
	*   placed in district (row, col)
	*
	* return:  indicates success/failure
	*/
	bool birth(int row, int col, int &id) {
		if (row < wRow && col < wCol)
		{
			
			person tmpPerson;
			tmpPerson.alive = true;
	
			tmpPerson.col = col;
			tmpPerson.row = row;
			node *tmp = new node;
			wPop++;

			if (deadPool.dfront == nullptr)
			{
				id = tmp->id  = idNum;
				push_back(tmp, row, col, id);
				tmpPerson.place = tmp;
				pVector.push_back(tmpPerson);
				idNum++;
			}
			else
			{
				id =tmp->id = deadPool.dfront->id;
				pVector[deadPool.dfront->id] = tmpPerson;
				push_back(tmp, row, col, id);
				deadPool.dfront = deadPool.dfront->next;
				deadPool.dPop--;
			}
			
			return true;
		}
		return false;
	}

	
	/*
	* function: death
	* description:  if given person is alive, person is killed and
	*   data structures updated to reflect this change.
	*
	* return:  indicates success/failure
	*/
	bool death(int personID) {
		if (personID < idNum && pVector[personID].alive)
		{
			//kills person
			pVector[personID].alive = false;
			//move to deadPool
			pVector[personID].place->prev = pVector[personID].place->next;
			deadPool.dPop++;

			if (deadPool.dfront == nullptr)
			{
				deadPool.dfront = deadPool.dback= pVector[personID].place;
				deadPool.dback->prev = nullptr;
				deadPool.dback->next = nullptr;
			}
			else
			{
				node *remind;
				deadPool.dback->next = pVector[personID].place;
				remind = deadPool.dback;
				deadPool.dback = deadPool.dback->next;
				deadPool.dback->prev = remind;
				deadPool.dback->next = nullptr;
			}
			dArr[pVector[personID].row][pVector[personID].col].dPop--;
			wPop--;
			return true;
		}
		return false;
	}

	/*
	* function: whereis
	* description:  if given person is alive, his/her current residence
	*   is reported via reference parameters row and col.
	*
	* return:  indicates success/failure
	*/
	bool whereis(int id, int &row, int &col)const {
		if (pVector[id].alive && id <= idNum)
		{
			row = pVector[id].row;
			col = pVector[id].col;
			return true;
		}
		else
			return false;
	}

	/*
	* function: move
	* description:  if given person is alive, and specified target-row
	*   and column are valid, person is moved to specified district and
	*   data structures updated accordingly.
	*
	* return:  indicates success/failure
	*
	* comment/note:  the specified person becomes the 'newest' member
	*   of target district (least seniority) --  see requirements of members().
	*/
	bool move(int id, int targetRow, int targetCol) {
		if (id < idNum && pVector[id].alive && targetRow < wRow && targetCol < wCol)
		{
			pVector[id].place->prev = pVector[id].place->next;
			dArr[pVector[id].row][pVector[id].col].dPop--;
			pVector[id].col = targetCol;
			pVector[id].row = targetRow;
			push_back(pVector[id].place, targetRow, targetCol, id);
			return true;
		}
		return false;
	}

	std::vector<int> * members(int row, int col)const {
		vector<int> *members = new vector<int>;
		node *header = dArr[row][col].dfront;
		if (row < wRow && col < wCol)
		{
			if (dArr[row][col].dPop > 0)
			{
				for (int i = 0; header != nullptr && i < dArr[row][col].dPop; i++, header = header->next)
					members->push_back(header->id);
			}			
			return members;
		}
		else
			return members;
	}

	/*
	* function: population
	* description:  returns the current (living) population of the world.
	*/
	int population()const {
		return wPop;
	}

	/*
	* function: population(int,int)
	* description:  returns the current (living) population of specified
	*   district.  If district does not exist, zero is returned
	*/
	int population(int row, int col)const {
		return dArr[row][col].dPop;
	}

	/*
	* function: num_rows
	* description:  returns number of rows in world
	*/
	int num_rows()const {
		return wRow;
	}

	/*
	* function: num_cols
	* description:  returns number of columns in world
	*/
	int num_cols()const {
		return wCol;
	}



};

#endif
