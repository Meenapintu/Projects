/*****************************************************************Sudoku Solver**************************************************************/
#include<simplecpp>
struct block {                                                          //define a structure block for storing information about the block
	int finalValue,possibleValueCount;                                  //value of block and number of possible values
	bool possibleValues[10];                                            //values possible for the block
	
	block() { };
	
	void initializeBlock() {                                            //initialize the block
		finalValue = 0;
		possibleValues[0] = false;
		for(int i=1;i<10;i++) possibleValues[i] = true;
		possibleValueCount = 9;
	}
	
	void takeInput(int inputNumber) {                                   //take input in block
		if(finalValue==0 && inputNumber>0 && inputNumber<10) {
			finalValue = inputNumber;
			for(int i=1;i<10;i++) possibleValues[i] = false;
			possibleValues[inputNumber] = true;
			possibleValueCount = 1;
		}
	}
};

bool checkBlock(int rowNumber,int columnNumber,block SudokuGrid[][10]) {//check the block if it has only one possible value
	if(SudokuGrid[rowNumber][columnNumber].possibleValueCount==1) {
		for(int i=1;i<10;i++) {
			if(SudokuGrid[rowNumber][columnNumber].possibleValues[i]) {
				SudokuGrid[rowNumber][columnNumber].finalValue = i;
				return true;
			}
		}
	}
	return false;
}

bool sudokuSolved(block SudokuGrid[][10]) {                             //check if the sudoku has been solved
	for(int i=1;i<10;i++) {
		for(int j=1;j<10;j++) {
			if(SudokuGrid[i][j].finalValue==0) return false;
		}
	}
	return true;
}

bool rowElimination(int rowNumber,block SudokuGrid[][10]) {             //check row and modify possible values for each block
	bool change = false;;
	for(int i=1;i<10;i++) {
		if(SudokuGrid[rowNumber][i].finalValue!=0) {
			int temp = SudokuGrid[rowNumber][i].finalValue;
			
			for(int j=1;j<10;j++) {
				if(SudokuGrid[rowNumber][j].possibleValues[temp] && SudokuGrid[rowNumber][j].finalValue==0) {
					    SudokuGrid[rowNumber][j].possibleValues[temp] = false;
					    SudokuGrid[rowNumber][j].possibleValueCount = SudokuGrid[rowNumber][j].possibleValueCount - 1;
				}
			}
		}
	}
	
	for(int i=1;i<10;i++) {                                             //check row for confirmed blocks
		if(SudokuGrid[rowNumber][i].finalValue==0) {
			change = change || checkBlock(rowNumber,i,SudokuGrid);
		}
	}
	return change;
}

bool columnElimination(int columnNumber,block SudokuGrid[][10]) {       //check column and modify possible values for each block
	bool change = false;;
	for(int i=1;i<10;i++) {
		if(SudokuGrid[i][columnNumber].finalValue!=0) {
			int temp = SudokuGrid[i][columnNumber].finalValue;
			
			for(int j=1;j<10;j++) {
				if(SudokuGrid[j][columnNumber].possibleValues[temp] && SudokuGrid[j][columnNumber].finalValue==0) {
					SudokuGrid[j][columnNumber].possibleValues[temp] = false;
					SudokuGrid[j][columnNumber].possibleValueCount = SudokuGrid[j][columnNumber].possibleValueCount - 1;
				}
			}
		}
	}
	
	for(int i=1;i<10;i++) {                                             //check column for confirmed blocks
		if(SudokuGrid[i][columnNumber].finalValue==0) {
			change = change || checkBlock(i,columnNumber,SudokuGrid);
		}
	}
	return change;
}

bool minigridElimination(int gridStartRow,int gridStartColumn,block SudokuGrid[][10]) {//check minigrid and modify possible values for each block
	bool change = false;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(SudokuGrid[gridStartRow+i][gridStartColumn+j].finalValue!=0) {
				int temp = SudokuGrid[gridStartRow+i][gridStartColumn+j].finalValue;
				
				for(int k=0;k<3;k++) {
					for(int l=0;l<3;l++) {
						if(SudokuGrid[gridStartRow+k][gridStartColumn+l].possibleValues[temp] && SudokuGrid[gridStartRow+k][gridStartColumn+l].finalValue==0) {
							SudokuGrid[gridStartRow+k][gridStartColumn+l].possibleValues[temp] = false;
							SudokuGrid[gridStartRow+k][gridStartColumn+l].possibleValueCount = SudokuGrid[gridStartRow+k][gridStartColumn+l].possibleValueCount - 1;
						}
					}
				}
			}
		}
	}
	
	for(int i=0;i<3;i++) {                                              //check minigrid for confirmed blocks
		for(int j=0;j<3;j++) {
			if(SudokuGrid[gridStartRow+i][gridStartColumn+j].finalValue==0) {
				change = change || checkBlock(gridStartRow+i,gridStartColumn+j,SudokuGrid);
			}
		}
	}
	return change;
}

bool applyRowColumnMinigridElimination(block SudokuGrid[][10]) {        //apply above three functions for the entire grid
	bool change = false;
	for(int i=1;i<10;i++) change = change || rowElimination(i,SudokuGrid);
	for(int j=1;j<10;j++) change = change || columnElimination(j,SudokuGrid);
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) change = change || minigridElimination((i*3)+1,(j*3)+1,SudokuGrid);
	}
	return change;
}
/*A lone ranger is a value for a block which does not occur in any other block's possible value in a row/column/minigrid*/
bool scanLoneRangerInRows(int rowNumber,block SudokuGrid[][10]) {       //scan for lone ranger in rows
	bool change = false;
	for(int i=1;i<10;i++) {
		if(SudokuGrid[rowNumber][i].finalValue==0) {
			for(int j=1;j<10 && (SudokuGrid[rowNumber][i].finalValue==0);j++) {
				if(SudokuGrid[rowNumber][i].possibleValues[j]) {
					bool loneRangerFound = true;
					for(int k=1;k<10 && loneRangerFound;k++) {
						if(k!=i) loneRangerFound = loneRangerFound && (SudokuGrid[rowNumber][k].possibleValues[j]==false);
					}
					if(loneRangerFound) {
						SudokuGrid[rowNumber][i].takeInput(j);
						change = true;
					}
				}
			}
		}
	}
	return change;
}

bool scanLoneRangerInColumns(int columnNumber,block SudokuGrid[][10]) { //scan for lone ranger in column
	bool change = false;
	for(int i=1;i<10;i++) {
		if(SudokuGrid[i][columnNumber].finalValue==0) {
			for(int j=1;j<10 && (SudokuGrid[i][columnNumber].finalValue==0);j++) {
				if(SudokuGrid[i][columnNumber].possibleValues[j]) {
					bool loneRangerFound = true;
					for(int k=1;k<10 && loneRangerFound;k++) {
						if(k!=i) loneRangerFound = loneRangerFound && (SudokuGrid[k][columnNumber].possibleValues[j]==false);
					}
					if(loneRangerFound) {
						SudokuGrid[i][columnNumber].takeInput(j);
						change = true;
					}
				}
			}
		}
	}
	return change;
}

bool scanLoneRangerInMinigrids(int gridStartRow,int gridStartColumn,block SudokuGrid[][10]) { //scan for lone ranger in minigrids
	bool change = false;
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			if(SudokuGrid[gridStartRow+i][gridStartColumn+j].finalValue==0) {
				for(int k=1;k<10 && (SudokuGrid[gridStartRow+i][gridStartColumn+j].finalValue==0);k++) {
					if(SudokuGrid[gridStartRow+i][gridStartColumn+j].possibleValues[k]) {
						bool loneRangerFound = true;
						for(int l=0;l<3;l++) {
							for(int m=0;m<3 && loneRangerFound;m++) {
								if(l!=i || m!=j) {
								    loneRangerFound = loneRangerFound && (SudokuGrid[gridStartRow+l][gridStartColumn+m].possibleValues[k]==false);
								}
							}
						}
						if(loneRangerFound) {
							SudokuGrid[gridStartRow+i][gridStartColumn+j].takeInput(k);
							change = true;
						}
					}
				}
			}
		}
	}
	return change;
}

bool solveSudokuByLogic(block SudokuGrid[][10]) {                       //use all the above functions to solve the sudoku
	bool change = true;
	while(true) {
		while(change) {
		    change = applyRowColumnMinigridElimination(SudokuGrid);
		}
	
	    for(int i=1;i<10;i++) change = change || scanLoneRangerInRows(i,SudokuGrid);
	    if(change) continue;
	
	    for(int i=1;i<10;i++) change = change || scanLoneRangerInColumns(i,SudokuGrid);
	    if(change) continue;
	
	    for(int i=0;i<3;i++) {
		    for(int j=0;j<3;j++) change = change || scanLoneRangerInMinigrids((i*3)+1,(j*3)+1,SudokuGrid);
		}
	    if(change) continue;

	    if(!change) break;
	}
	if(sudokuSolved(SudokuGrid)) return true;
	return false;
}

bool solveSudokuByBruteForce(block SudokuGrid[][10]) {//if the sudoku is not solvable by logic solve it by guessing and subsequent solving by logic
	for(int i=1;i<10;i++) {
		for(int j=1;j<10;j++) {
			if(SudokuGrid[i][j].possibleValueCount==2) {
				
				for(int m=1;m<10;m++) {
					if(SudokuGrid[i][j].possibleValues[m]) {
						block gridcopy[10][10];
						for(int k=1;k<10;k++) {
							for(int l=1;l<10;l++) {
								gridcopy[k][l] = SudokuGrid[k][l];
							}
						}
						gridcopy[i][j].takeInput(m);
						bool solved = solveSudokuByLogic(gridcopy);
						if(solved) {
							for(int k=1;k<10;k++) {
								for(int l=1;l<10;l++) {
									SudokuGrid[k][l] = gridcopy[k][l];
								}
							}
							return true;
						}
					}
				}
			}
		}
	}		
	return false;
}

void rectangle(int &cx,int &cy,int l,int h,int c1,int c2,int c3) {
	Rectangle r(cx,cy,l,h);
	r.setColor(COLOR(c1,c2,c3));
	r.setFill(); r.imprint();
	return;
}

void drawGrid(int n) {
	for(int i=0;i<=n;i++) {
	    Line l(40,40+40*i,40+40*n,40+40*i); 
	    l.setColor(COLOR("WHITE"));
	    l.imprint();
	    l.reset(40+40*i,40,40+40*i,40+40*n); 
	    l.imprint();
	}
}

int main() {
	block blocksOfSudokuGrid[10][10];
	for(int i=1;i<10;i++) {
		for(int j=1;j<10;j++) {
			blocksOfSudokuGrid[i][j].initializeBlock();
		}
	}
	
	initCanvas("Sudoku Solver",500,500);
	Rectangle interface(0,0,0,0);
	for(int x=0;x<251;x++) {
		interface.reset(250,250,500-2*x,500-2*x);
		interface.setColor(COLOR(0,x,x/2.0));
		interface.imprint();
	}
	
	int m,n;
	drawGrid(9);
	
	int p=450,q=100;
	rectangle(p,q,80,40,255,255,255);
	Text t(p,q,"EXIT");
	t.imprint();
	
	p=450; q=200;
	rectangle(p,q,80,40,255,255,255);
	t.reset(p,q,"DONE");
	t.imprint(); 
	
	XEvent event; {
	    while(true) {
		    nextEvent(&event);
		    int x,y;
		    if(mouseButtonPressEvent(&event)) {
		        int coordinate = getClick();
		        x = coordinate/65536; y = coordinate%65536;
		       		        
		        n = (x - 40)/40 ; m = (y-40) / 40;
			
				x = 40+(n+0.5)*40; 
				y = 40+(m+0.5)*40;
				
				if(x<=400 && x>=40 && y<=400 && y>=40) rectangle(x,y,38,38,255,70,0);
				
				if(x<=490 && x>=410 && y<=220 && y>=180) {
							bool yourSudokuSolved = solveSudokuByLogic(blocksOfSudokuGrid);
	                        if(!yourSudokuSolved) solveSudokuByBruteForce(blocksOfSudokuGrid);
	                        break;
						}
					}
			
				if(keyPressEvent(&event)) {
			        char inputNumber = charFromEvent(&event);
					rectangle(x,y,38,38,0,0,255);
					t.reset(x,y,inputNumber-48);
					t.setColor(COLOR(255,255,255));
					t.imprint();
					if(inputNumber==48) t.hide();
					
					blocksOfSudokuGrid[n+1][m+1].takeInput(inputNumber-48);
				}
			}
		}
	
	for(int i=1;i<10;i++) {
		for(int j=1;j<10;j++) {
			char c = blocksOfSudokuGrid[i][j].finalValue;
			Text t(20+40*i,20+40*j,c); t.setColor(COLOR(255,255,255)); t.imprint(); 
		}
	}
	int coordinate = getClick();
	int a=coordinate/65536; int b=coordinate%65536;
	if (a<=490 && a>=410 && b<=110 && b>=80) closeCanvas();
}
