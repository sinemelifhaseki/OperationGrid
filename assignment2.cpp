#include <cstdio>
#include <cstdlib>
#include <iostream>
#define MAX_OPERATOR_SIZE 100
using namespace std;

int errorcheck(char **grid, int gridrows, int gridcols, int center_x, int center_y, int size, char sign);
bool conflicterror(char **currentgrid, int row, int col);
void filloperator(char **grid, char sign, int center_x, int center_y, int size, bool delete_mode);
bool conflicterror(char **currentgrid, int row, int col);
bool bordererror(int gridrows, int gridcols, int row, int col);
int inserror(char **grid, int gridrows, int gridcols, int moveby, int flag, int center_x, int center_y, char sign, int size);

class Operator{
    int center_x;
    int center_y;
    int op_size;
public:
    Operator(int x, int y, int size);

    void reset(int new_x, int new_y, int new_size);
    
    void set_x(int new_x);
    int get_x();
    void set_y(int new_y);
    int get_y();
    void set_size(int new_size);
    int get_size();
};

Operator::Operator(int x, int y, int size){
    center_x = x;
    center_y = y;
    op_size = size;
}

void Operator::reset(int new_x, int new_y, int new_size){
    center_x = new_x;
    center_y = new_y;
    op_size = new_size;
}
//getter&setters
void Operator::set_x(int new_x){
    center_x = new_x;
}
int Operator::get_x(){
    return center_x;
}
void Operator::set_y(int new_y){
    center_y = new_y;
}
int Operator::get_y(){
    return center_y;
}
void Operator::set_size(int new_size){
    op_size = new_size;
}
int Operator::get_size(){
    return op_size;
}

class ArithmeticOperator: public Operator{
    char sign;
public:
    ArithmeticOperator(int x, int y, int size, char sign);
    ArithmeticOperator();
    char get_sign();
    //Prints out operator's center location, size and sign character
    void print_operator();
};
//functions
ArithmeticOperator::ArithmeticOperator(int x, int y, int size, char sign):Operator(x,y,size){
	if(sign == '+' || sign == '-' || sign == 'x' || sign == '/'||sign == ' ')
        this->sign = sign;
    else 
        cout<< "SIGN parameter is invalid!"<<endl;
}
char ArithmeticOperator::get_sign(){
    return sign;
}
void ArithmeticOperator::print_operator(){
    cout<<"ARITHMETIC_OPERATOR["<<get_sign()<<"], CENTER_LOCATION["<<get_x()<<","<<get_y()<<"], SIZE["<<get_size()<<"]"<<endl;
}

class OperatorGrid{
    int grid_rows;
    int grid_cols;
    char **grid;
    int num_operators;
    ArithmeticOperator *operators[MAX_OPERATOR_SIZE];
public:
    OperatorGrid(int rows, int cols);
    ~OperatorGrid();

    bool place_operator(ArithmeticOperator *);
    bool move_operator(int x, int y, char direction, int move_by);
    void print_operators();
};

OperatorGrid::OperatorGrid(int rows, int cols){
    num_operators = 0;
	for(int i = 0; i <MAX_OPERATOR_SIZE; i++)
    	operators[i] = new ArithmeticOperator(-1,-1,-1,' ');
    grid_rows = rows;
    grid_cols = cols;
    grid = new char*[grid_rows];
    for (int i = 0; i < grid_rows ; i++)
        grid[i] = new char[grid_cols];
    //initialize grid with space char
    for(int i = 0; i < grid_rows; i++){
        for(int j=0; j < grid_cols; j++){
            grid[i][j] = ' ';
    }}
}
OperatorGrid::~OperatorGrid(){
    for (int i = 0; i < grid_rows; i++) {
		delete grid[i];
	}
	delete[] grid;
	for(int i=0; i < MAX_OPERATOR_SIZE; i++)
    	delete operators[i];
    cout << "DESTRUCTOR: GIVE BACK["<<grid_rows<<","<<grid_cols<<"] chars."<<endl;
    cout << "DESTRUCTOR: GIVE BACK["<<num_operators<<"] Operators."<<endl;
}

void OperatorGrid::print_operators(){
	for (int i = 0; i< num_operators; i++)
		operators[i]->print_operator();
}
bool OperatorGrid::place_operator(ArithmeticOperator *oper){
    int size = oper->get_size();
    int center_x = oper->get_x();
    int center_y = oper->get_y();
    char sign = oper->get_sign();
    center_x--;
    center_y--;
    int flag = errorcheck(grid, grid_rows, grid_cols, center_x, center_y, size, sign);
    if (flag == 0) {
		    operators[num_operators] = oper;
			num_operators += 1;
			filloperator(grid, sign, center_x, center_y, size, false);
			cout << "SUCCESS: Operator " << sign << " with size " << size << " is placed on (" << center_x + 1 << "," << center_y + 1 << ")." << endl;
			return true;
		}
		else if (flag == 1) {
			cout << "BORDER ERROR: Operator " << sign << " with size " << size << " can not be placed on (" << center_x + 1 << ',' << center_y + 1 << ")." << endl;
		}
		else if (flag == 2) {
			cout << "CONFLICT ERROR: Operator " << sign << " with size " << size << " can not be placed on (" << center_x + 1 << ',' << center_y + 1 << ")." << endl;
		}
		else if (flag == 3) {
			cout << "BORDER ERROR: Operator " << sign << " with size " << size << " can not be placed on (" << center_x + 1 << ',' << center_y + 1 << ")." << endl;
			cout << "CONFLICT ERROR: Operator " << sign << " with size " << size << " can not be placed on (" << center_x + 1 << ',' << center_y + 1 << ")." << endl;
		}
	return false;
}

//PLACE OPERATIONS
int errorcheck(char **grid, int gridrows, int gridcols, int center_x, int center_y, int size, char sign){
    bool border_flag = false, conflict_flag = false;
    int i;
    //checks the errors for all grid blocks of one operator
    switch (sign) {
	case '+':
		for (i = center_x - size; i < center_x + size + 1; i++) {
			if (bordererror(gridrows,gridcols,i, center_y) == false) {
				if (conflicterror(grid, i, center_y) == true) {
					conflict_flag = true;
				}
			}
			else {
				border_flag = true;
			}
		}

		for (i = center_y - size; i < center_y + size + 1; i++) {

			if (bordererror(gridrows,gridcols,center_x, i) == false) {
				if (conflicterror(grid, center_x, i) == true) {
					conflict_flag = true;
				}
			}
			else {
				border_flag = true;
			}
		}

		break;
	case '-':
		for (i = center_y - size; i < center_y + size + 1; i++) {

			if (bordererror(gridrows,gridcols,center_x, i) == false) {
				if (conflicterror(grid, center_x, i) == true) {
					conflict_flag = true;
				}
			}
			else
				border_flag = true;
			}
			break;
	case 'x':
		for (i = -size; i < size + 1; i++) {

			if (bordererror(gridrows,gridcols,center_x + i, center_y + i) == false) {
				if (conflicterror(grid,center_x + i, center_y + i) == true) {
					conflict_flag = true;
				}
			}
			else {
				border_flag = true;
			}
		}

		for (i = -size; i < size + 1; i++) {

			if (bordererror(gridrows,gridcols,center_x + i, center_y - i) == false) {
				if (conflicterror(grid, center_x + i, center_y - i) == true) {
					conflict_flag = true;
				}
			}
			else {
				border_flag = true;
			}
		}

		break;
	case '/':

		for (i = -size; i < size + 1; i++) {

			if (bordererror(gridrows,gridcols,center_x + i, center_y - i) == false) {
				if (conflicterror(grid,center_x + i, center_y - i) == true) {
					conflict_flag = true;
				}
			}
			else {
				border_flag = true;
			}
		}


		break;
		}
		if (!border_flag && !conflict_flag) {
			return 0;
		}
		else if (border_flag) {
			if (!conflict_flag) {
				return 1;
			}
			return 3;
		}
		else if (conflict_flag) {
			return 2;
		}
	return 0;
}
bool bordererror(int gridrows, int gridcols, int row, int col){ //checks border error block by block
    if (row<0 || row>gridrows - 1 || col<0 || col>gridcols - 1)
		return true;
	return false;
}
bool conflicterror(char **currentgrid, int row, int col){ //checks conflict error block by block
    if (currentgrid[row][col] == ' ')
		return false;
	return true;
}

void filloperator(char **grid, char sign, int center_x, int center_y, int size, bool delete_mode) { // fills the operators based on info on grid file and inst file, it also has a delete_mode which is for deleting the info before moving operations
	switch (sign) {
	case '+':
		for (int i = center_x - size; i < center_x + size + 1; i++) {//if deletemode parameter is sent in as true, it deletes the operator
			if (!delete_mode)
				grid[i][center_y] = '+';
			else {
				grid[i][center_y] = ' ';
			}
		}

		for (int i = center_y - size; i < center_y + size + 1; i++) {
			if (!delete_mode)
				grid[center_x][i] = '+';
			else {
				grid[center_x][i] = ' ';
			}
		}

		break;
	case '-':
		for (int i = center_y - size; i < center_y + size + 1; i++) {
			if (!delete_mode)
				grid[center_x][i] = '-';
			else {
				grid[center_x][i] = ' ';
			}
		}
		break;

	case 'x':
		for (int i = -size; i < size + 1; i++) {
			if (!delete_mode)
				grid[center_x + i][center_y + i] = 'x';
			else {
				grid[center_x + i][center_y + i] = ' ';
			}
		}

		for (int i = -size; i < size + 1; i++) {
			if (!delete_mode)
				grid[center_x + i][center_y - i] = 'x';
			else {
				grid[center_x + i][center_y - i] = ' ';
			}
		}

		break;
	case '/':

		for (int i = -size; i < size + 1; i++) {
			if (!delete_mode)
				grid[center_x + i][center_y - i] = '/';
			else {
				grid[center_x + i][center_y - i] = ' ';
			}
		}
		break;
	}
}

//MOVE OPERATIONS
int inserror(char **grid, int gridrows, int gridcols, int moveby, int flag, int center_x, int center_y, char sign, int size) {//checks whether there will be error when the wanted operation is done
	char tempsign = sign;
	filloperator(grid, sign, center_x, center_y, size, true);

	int result = 0, r_new, c_new;

	if (flag == 1) { //MOVE LEFT
		result = errorcheck(grid, gridrows, gridcols, center_x , center_y - moveby, size, tempsign);
		r_new = center_x +1;
		c_new = center_y+1 -moveby;
	}
	else if (flag == 2) {//MOVE RIGHT
		result = errorcheck(grid, gridrows, gridcols, center_x, center_y + moveby, size, tempsign);
		r_new = center_x +1;
		c_new = center_y + moveby +1;
	}
	else if (flag == 3) {//MOVE UP
		result = errorcheck(grid, gridrows, gridcols, center_x - moveby, center_y, size, tempsign);
		r_new = center_x - moveby +1;
		c_new = center_y +1;
	}
	else if (flag == 4) {//MOVE DOWN
		result = errorcheck(grid, gridrows, gridcols, center_x + moveby, center_y, size, tempsign);
		r_new = center_x + moveby +1;
		c_new = center_y +1;
	}

	if (result == 0) {
		filloperator(grid, sign, r_new-1, c_new-1, size, false);
		cout << "SUCCESS: " << sign << " moved from (" << center_x + 1 << "," << center_y +1<< ") to (" << r_new  << "," << c_new  << ")." << endl;
	}
	else if (result == 1) {
		cout << "BORDER ERROR: " << tempsign << " can not be moved from (" << center_x+1 << "," << center_y +1 << ") to (" << r_new  << "," << c_new  << ")." << endl;
		filloperator(grid, tempsign, center_x, center_y, size, false);
	}
	else if (result == 2) {
		cout << "CONFLICT ERROR: " << tempsign << " can not be moved from (" << center_x +1<< "," << center_y +1 << ") to (" << r_new  << "," << c_new << ")." << endl;
		filloperator(grid, tempsign, center_x, center_y, size, false);
	}
	else if (result == 3) {
		cout << "BORDER ERROR: " << tempsign << " can not be moved from (" << center_x +1 << "," << center_y +1 << ") to (" << r_new  << "," << c_new  << ")." << endl;
		cout << "CONFLICT ERROR: " << tempsign << " can not be moved from (" << center_x +1 << "," << center_y +1 << ") to (" << r_new  << "," << c_new  << ")." << endl;
		filloperator(grid, tempsign, center_x, center_y, size, false);
	}
	return result;
}
bool OperatorGrid::move_operator(int x, int y, char direction, int move_by){
	x--;
	y--;
	bool found = 0;
	int flag, tempx,i, j, tempy;
	int center_x, center_y, size;
	char sign = grid[x][y];
	//for finding the center of the given instruction parameter
	for(i = 0; i < num_operators; i++){
		if(sign == operators[i]->get_sign()){
			switch (sign){
				case '+': //if given coordinates is +
					for (j = operators[i]->get_x()-1 - operators[i]->get_size(); j <= operators[i]->get_x()-1 + operators[i]->get_size(); j++) {
						if(j==x){
							for (j = operators[i]->get_y()-1 - operators[i]->get_size(); j <= operators[i]->get_y()-1 + operators[i]->get_size(); j++) {
								if(j == y){
									center_y = operators[i]->get_y()-1;
									center_x = operators[i]->get_x()-1;
									found = 1;
									size = operators[i]->get_size();
									break;
						}
					}
						break;
						}
					}
					
					break;

				case '-': //if given coordinates is -
					for (j = operators[i]->get_y()-1 - operators[i]->get_size(); j <= operators[i]->get_y() -1 + operators[i]->get_size(); j++){
						if(j == y){
							
								if(operators[i]->get_x()-1 == x){
									center_y = operators[i]->get_y()-1;
									center_x = operators[i]->get_x()-1;
									found = 1;
									size = operators[i]->get_size();
									break;
								}
							break;
						}
					}	
					break;

				case 'x': ////if given coordinates is x
					for (j = - operators[i]->get_size(); j< operators[i]->get_size() + 1; j++){
						if(j+operators[i]->get_x()-1 == x){
							center_x = operators[i]->get_x()-1;
							break;
						}
					}
					for (j = - operators[i]->get_size(); j < operators[i]->get_size() + 1; j++){
						if(operators[i]->get_y()-1 - j == y){
							center_y = operators[i]->get_y()-1;
							size = operators[i]->get_size();
							found = 1;
							break;
						}
					}
					break;
			
				case '/': //if given coordinates is /
					for (j = 0; j < operators[i]->get_size() + 1; j++){ //for upper part of /
						if(operators[i]->get_x()-1 - j == x){
							for (int k = 0; k < operators[i]->get_size() + 1; k++){
								if(operators[i]->get_y()-1 + k == y){
									center_x = operators[i]->get_x()-1;
									center_y = operators[i]->get_y()-1;
									size = operators[i]->get_size();
									found = 1;
									break;
						}}
							break;	
					}
					}
					if (!found){ // for lower part of / operator
						for (j = 0; j < operators[i]->get_size() + 1; j++){ //for upper part of / operator
							if(operators[i]->get_x()-1 + j == x){
								for (int k = 0; k < operators[i]->get_size() + 1; k++){
									if(operators[i]->get_y()-1 - k == y){
										center_x = operators[i]->get_x()-1;
										center_y = operators[i]->get_y()-1;
										size = operators[i]->get_size();
										found = 1;
										break;
						}}
							break;	
						}
						}
					}
			
			}}
			if(found) break;
	}

//checking if there will be an error after move operation
	if (direction == 'L'){
		flag = inserror(grid, grid_rows,grid_cols,move_by, 1,center_x, center_y, sign, size);
		if(flag == 0) {
			operators[i]->set_y(center_y+1 - move_by);
			return true;}
	}
	else if (direction == 'R'){
		flag = inserror(grid, grid_rows,grid_cols,move_by, 2,center_x, center_y, sign, size);
		if(flag == 0) {
			operators[i]->set_y(center_y+1 + move_by);
			return true;}
	}
	else if (direction == 'U'){
		flag = inserror(grid, grid_rows,grid_cols,move_by, 3,center_x, center_y, sign, size);
		if(flag == 0) {
			operators[i]->set_x(center_x+1 - move_by);
			return true;}
	}
	else if (direction == 'D'){
		flag = inserror(grid, grid_rows,grid_cols,move_by, 4,center_x, center_y, sign, size);
		if(flag == 0) {
			operators[i]->set_x(center_x+1+move_by);
			return true;}
	}
	return false;
}