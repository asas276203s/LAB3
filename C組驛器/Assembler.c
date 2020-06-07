#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct string {
	char symbol[7];
	char mark1;
	char mark2;
	char op[6];
	char operand1[8];
	char operand2[9];
	char EQU[20];
	int location;
	struct string *next;
}oper;

oper lit_tab[20];
oper sym_tab[20], b[20];
char op_tab[21][3][10]={{"J","3/4","3C"},{"JEQ","3/4","30"},{"JSUB","3/4","48"},
	{"LDA","3/4","00"},{"LDT","3/4","74"},{"LDB","3/4","68"},{"LDCH","3/4","50"},
	{"RD","3/4","D8"},{"WD","3/4","DC"},{"TD","3/4","E0"},
	{"RSUB","3/4","4C"},{"JLT","3/4","38"},
	{"STA","3/4","0C"},{"STL","3/4","14"},{"STCH","3/4","54"},{"STX","3/4","10"},
	{"COMP","3/4","28"},{"COMPR","2","A0"},{"CLEAR","2","B4"},
	{"TIXR","2","B8"},{"TIX","3/4","2C"}};
char reg_tab[10][2][2] = {{"A","0"}, {"X","1"}, {"L","2"}, {"B","3"}, {"S","4"}, {"T","5"}, {"F","6"}, {"PC","8"}, {"SW","9"}};
char reserve[2][10] = {"RESW", "RESB"};
char instructor[10][10] = {"LTORG", "BASE"};

char* string(char input[], int first, int end) {
	char *op = malloc(20 * sizeof(char));
	memset(op, '\0',20);
	int i = 0;
	while(input[first] != input[6] && first < end) {
		op[i++] = input[first];
		first++;
	}
	
	return op;
}
int oplocation(oper *first, int location) { //格式三 3Bytes 
	oper *ptr = first;
	int i;
	while(ptr->next != NULL) {
		ptr = ptr->next;
	}
	for(i=0; i<21; i++) { //optab
		if(strcmp(op_tab[i][0], ptr->op) == 0 && op_tab[i][1][0] != '2') {
			location += 3;
			break;
		}
		else if (strcmp(op_tab[i][0], ptr->op) == 0 && op_tab[i][1][0] == '2') {
			location += 2;
			break;
		}
	}
	if(strcmp(ptr->op, "RESW") == 0) {
		int num = atoi(ptr->operand1)*3;
		location += num;
	}
	else if (strcmp(ptr->op, "RESB") == 0) {
		location += atoi(ptr->operand1);
	}
	else if (strcmp(ptr->op, "BYTE") == 0) {
		if(ptr->operand1[2] == 'F' && ptr->operand1[3] == '1')
			location += 1;
	}
		
	return location;
}
void buble_sort(int sym_num, oper *first) {
	int j=0,k=0,i=0,t=0,n=0;
	oper *ptr = first;
	for(i=0; i<sym_num; i++) {
		if(n < strlen(sym_tab[i].symbol))
			n=strlen(sym_tab[i].symbol);
	}
	for(i=0;i<sym_num;i++)
	{ 
		for(j=0;j<sym_num-i;j++)
	    	if(strcmp(sym_tab[j].symbol,sym_tab[j+1].symbol)>0)
	 		{
	  			for(k=0;k<n;k++)
	  				b[j].symbol[k]=sym_tab[j].symbol[k];
	  			for(k=0;k<n;k++)
	  				sym_tab[j].symbol[k]=sym_tab[j+1].symbol[k];
	  			for(k=0;k<n;k++)
	  				sym_tab[j+1].symbol[k]=b[j].symbol[k];  
	     	}	      
	}
	
	for(i=1; i<=sym_num; i++) {
		ptr = first;
		while(ptr != NULL) {
			if(strcmp(ptr->symbol, sym_tab[i].symbol) == 0) {
				sym_tab[i].location = ptr->location;
			}
			ptr = ptr->next;
		}
	}
}
char* obj(int sym_num, oper *input) {
	
}
int main()
{
	int location = 0;
    char *op, *sym, *operand1, *operand2, *EQU;
	int sym_num = 0,lit_num = 0;
	int i=0;
	oper *first = NULL, *ptr, *end;
	
	FILE* fp_r = fopen("2.9.txt","r");
    FILE* fp_w = fopen("write.txt","w");
    if(fp_r == NULL || fp_w == NULL)
    {
        printf("開檔失敗!\n");
        return -1;
    }
    char input[33];
    while(fgets(input, 33,fp_r)!= NULL) {
    	char tmp[10];
    	int len = strlen(input);
    	len -= 1;
    	
		op = string(input, 8, len);
    	sym = string(input, 0, len);
    	if(strcmp(sym, "FIRST") == 0) {
			strcpy(tmp, "FIRST");
			memset(sym, '\0',20);
		}
		else if(strcmp(sym, "COPY") == 0) {
			strcpy(tmp, "COPY");
			memset(sym, '\0',20);
		}
    	end = (oper*)malloc(sizeof(oper));
    	memset(end->EQU, '\0',20);
    	memset(end->operand2, '\0',20);
    	end->mark2 = input[15];			 //mark2
		end->mark1 = input[7];           //mark1
    	strcpy(end->op, op);  // op
    	strcpy(end->symbol, sym); //symbol
    	
		if(strlen(end->symbol)) { // symbol table
    		strcpy(sym_tab[sym_num++].symbol, end->symbol);
    	}
		if(strcmp(tmp, "FIRST") == 0)
			strcpy(end->symbol, tmp);
		else if(strcmp(tmp, "COPY") == 0)
			strcpy(end->symbol, tmp);
		
		if(strcmp(end->op, "EQU") != 0)	{
			operand1 = string(input, 16, len); // operand1
			operand2 = string(input, 25, len); // operand2
		}
		else if(strcmp(end->op, "EQU") == 0) { // EQU
			EQU = string(input, 16, len);
			strcpy(end->EQU, EQU);
		}
		if(strlen(operand1)) { //operand1
    		strcpy(end->operand1, operand1);
		}
		
		if(strlen(operand2)) { //operand2
    		strcpy(end->operand2, operand2);
    	}
		if(input[15] == '=') {  // literal
			int flag = 1;
			for(i=0; i<lit_num; i++) {
				if(strcmp(lit_tab[i].op, end->operand1) == 0)
					flag = 0;
			}
			if(flag) { // lit_table
				lit_tab[lit_num].location = location;
				strcpy(lit_tab[lit_num].symbol, "*");
				strcpy(lit_tab[lit_num++].op, end->operand1);
			}
		}
		if(first == NULL) {
			first = end;
			ptr = end;
			end->next = NULL;
		}
		else {
			ptr->next = end;
			ptr = end;
			end->next = NULL;
		}
		 
		if(strcmp(end->op, "BASE") == 0 || strcmp(end->op, "LTORG") == 0 || strcmp(end->op, "END") == 0) {
			end->location = -1;
			if(strcmp(end->op, "LTORG") == 0) {
				for(i=0; i<lit_num; i++) {
					oper *add = (oper*)malloc(sizeof(oper));
					strcpy(add->op, lit_tab[i].op);
					strcpy(add->symbol, lit_tab[i].symbol);
					add->location = location;
					ptr->next = add;
					ptr = ptr->next;
					ptr->next = NULL;
					location += 3;
					memset(lit_tab[lit_num--].op, '\0',20);
				}
			}
		}
		else {
			if(strcmp(end->op, "EQU") == 0) {
				if(strcmp(end->EQU, "*") == 0) {
					end->location = location;
					end->operand1[0] = '*';
				}
				else {
					int j=0;
					int num1=0,num2=0;
					char tmp_EQU1[10], tmp_EQU2[10],tmp_EQU[1];
					while(j < strlen(end->EQU)) {
						if(j < 6) {
							tmp_EQU1[num1++] = end->EQU[j];
						}
						else if (j == 6)
							tmp_EQU[0] = end->EQU[j];
						else if(j > 6)
							tmp_EQU2[num2++] = end->EQU[j];
						j++;
					}
					oper *tmp_U = first;
					while(tmp_U != NULL) {
						if(strcmp(tmp_U->symbol, tmp_EQU1) == 0)
							num1 = tmp_U->location;
						else if(strcmp(tmp_U->symbol, tmp_EQU2) == 0)
							num2 = tmp_U->location;
						tmp_U = tmp_U->next;
					}
					end->location = (num1 - num2);
				}
			}
			else
				end->location = location;
		}
		
		if(input[7] == '+') {  //location
			location += 4;
		}
		else {
			location = oplocation(first, location); //location
		}
		memset(op, '\0',20);
    	memset(sym, '\0',20);
    	memset(operand1, '\0',20);
    	memset(operand2, '\0',20);
    	memset(tmp, '\0',10);
    }
    ptr->operand1[strlen(ptr->operand1)] = input[strlen(input)-1];
    for(i=0; i<lit_num; i++) {
		//printf("%s %x\n",lit_tab[i].op, lit_tab[i].location); lit
		oper *add = (oper*)malloc(sizeof(oper));
		strcpy(add->op, lit_tab[i].op);
		add->location = location;
		ptr->next = add;
		ptr = add;
		ptr->next = NULL; 
	}
	ptr = first;
	while(ptr != NULL) {
		if(ptr->location >= 0){  //base
			if(strcmp(ptr->op, "EQU") == 0)
				fprintf(fp_w, "%04x\t%-6s \t%-6s\t %-18s\n",ptr->location, ptr->symbol, ptr->op,ptr->EQU);
			else if(strlen(ptr->operand2))
				fprintf(fp_w, "%04x\t%-6s \t%-6s\t %-18s %-18s\n",ptr->location, ptr->symbol, ptr->op, ptr->operand1, ptr->operand2);
			else
				fprintf(fp_w, "%04x\t%-6s \t%-6s\t %-18s\n",ptr->location, ptr->symbol, ptr->op, ptr->operand1);
		}
			
		else
			fprintf(fp_w, "\t%-6s \t%-18s\n", ptr->op, ptr->operand1);
		ptr = ptr->next;
	}
	buble_sort(sym_num, first);
	
	for(i=1; i<=sym_num; i++) {    //1-13
		printf("%-6s %04x\n",sym_tab[i].symbol, sym_tab[i].location);
	}
	
    fclose(fp_r);
    fclose(fp_w);
    
    return 0;
}
