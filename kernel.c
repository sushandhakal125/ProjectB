//Functions
void printString(char* string);
void readString(char* string);
void readSector(char* buffer, int sector);

void handleInterrupt21(int ax, int bx, int cx, int dx);

int main() {

	//char line[80];
	//makeInterrupt21();
	//interrupt(0x21,1,line,0,0);
	//interrupt(0x21,0,line,0,0);
    
    char line[80];
	printString("Enter a line: ");
	readString(line);
	printString(line);

	while(1);
}

void printString(char* string){ 
	//print strings out the screen
	int position = 0;
	while(string[position] != '\0'){
		interrupt(0x10, 0xE*256+string[position], 0, 0, 0);
		position = position +1;
	}
}
void readString(char* string){
	//read strings from a user
	int position = 0;
	char character = 0x0;
	while(character != 0xd){
		character = interrupt(0x16, 0, 0, 0, 0);
		if(character == 0xd){
			break;
		}
		else if(character == 0x8){
			if(position > 1){
				position = position -1;
				interrupt(0x10, 0xE*256+character, 0, 0, 0);
			}
		}
		else {
			string[position] = character;
			position = position +1;
			interrupt(0x10, 0xE*256+character, 0, 0, 0);
		}
	}
	interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
	string[position] = 0xa;
	string[position+1]= 0x0;
}

void readSector(char* buffer, int sector){
	//read from a sector
	int relSec = sector+1;
	int head= 0;
	int track = 0;
	interrupt(0x13, 2*256+1, buffer, track*256+relSec, head*256+0);
	
}



void handleInterrupt21(int ax, int bx, int cx, int dx){
	//handles the interrupt I made
	int *bxp = bx;
	
	if(ax == 0){
		//this is for the printing strings
		while( *bxp != '\0'){
			interrupt(0x10, 0xE*256+*(bxp), 0, 0, 0);
			bxp= bxp+1;
		}
	} else if (ax == 1){
		//this is for reading strings
		int base =bx;
		char character = 0x0;
		while(character != 0xd){
			character = interrupt(0x16, 0, 0, 0, 0);
			if(character == 0xd){
				break;
			}
			else if(character == 0x8){
				if(bxp > base){
					bxp = bxp -1;
					interrupt(0x10, 0xE*256+character, 0, 0, 0);
				}
			}
			else {
				*(bxp) = character;
				bxp = bxp +1;
				interrupt(0x10, 0xE*256+character, 0, 0, 0);
			}
		}
		interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
		*(bxp) = 0xa;
		bxp = bxp+1;
		*(bxp) = 0x0;

	} else if (ax == 2){
		//reading from a sector
		interrupt(0x13, ax, bx, cx, 0);
	} else {
		printString("Error invalid input was entered");
	}
}
