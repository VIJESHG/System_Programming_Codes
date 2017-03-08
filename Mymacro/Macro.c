#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct MNT{
	char name[28];
	int start,pars;
}MNT;

typedef struct FP_table{
	char formal[28];
	int positional;
}FP_table;
typedef struct PA_table{
	int positional;
	char actual[28];
}PA_table;
typedef struct MDT{
	char data[100];
}MDT;

void Print_MDT(struct MDT a[],int size){
	int i;
	printf("-------MDT_TABLE-----------\n");
	for(i = 0; i < size; i++) {
		printf("%s",a[i].data);
	}
}
int Search(char a[],struct MNT b[],int size){
	int i = 0;
	while(i < size){
		if(strcmp(a,b[i].name) == 0) {
			return b[i].start;
		}
		else
			i++;
	}
	return -1;
}
void Print_MNT(struct MNT b[],int size){
	int i;
	printf("-------MNT_TABLE-----------\n");
	printf("NAME	START	PARS-----------\n");
	for(i = 0; i < size; i++) {
		printf("%s	%d	%d\n",b[i].name,b[i].start,b[i].pars);
	}
}
void Print_FP(struct FP_table c[],int size){
	int i;
	printf("-------FvsP_TABLE-----------\n");
	printf("FORMAL	POSI-----------\n");
	for(i = 0; i < size; i++) {
		printf("%s	#%d\n",c[i].formal,c[i].positional);
	}
}
void Print_PA(struct PA_table c[],int size){
	int i;
	printf("-------PvsA_TABLE-----------\n");
	printf("POSI	ACTUAL-----------\n");
	for(i = 0; i < size; i++) {
		printf("#%d	%s\n",c[i].positional,c[i].actual);
	}
}
void itoa(int value, char* str, size_t size) {
	snprintf(str, size, "%d", value);
}

char *strrpc(char *str, char *orig, char *rep){
  static char buffer[4096];
  char *p;
  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}

int main() {
	struct MNT A[10];
	struct MDT B[128];
	struct FP_table C[10];
	struct PA_table D[10];
	int i = 0,j = 0,k = 0,l,d_flag = 0,e_flag = 0,MDT_CNT = 1,par_CNT,r,n,m;
	char delim2[3] = " 	\n",str[128],*token,str1[128],str3[128],tmp[10];
	FILE *fp1,*fp2;
	fp1 = fopen("input.txt","r");
	fp2 = fopen("op.txt","w+");
	while(fgets(str,128,fp1)){
		strcpy(str3,str);
		token = strtok(str,delim2);				
		strcpy(str1,token);							
		if(strcmp(str1,"END") != 0){
			if(strcmp(str1,"MACRO")==0){
				d_flag = 1;
				par_CNT = 0;
				token = strtok(NULL,delim2);
				strcpy(A[i].name,token);
				A[i].start = MDT_CNT;
				token = strtok(NULL,delim2);
				l = 1;
				par_CNT = 0;
				while(token != NULL){
					par_CNT++;
					C[k].positional = l++;
					strcpy(C[k++].formal,token);
					token = strtok(NULL,delim2);
				}
				A[i].pars = par_CNT;
				i++;
			}
			else if(d_flag == 1){
				strcpy(B[j].data,str3);
				for(n = 0; n < par_CNT; n++){
					for(m = 0; m < j; m++){
						itoa(D[n].positional,tmp,10);
						strcpy(B[m].data,strrpc(B[m].data,tmp,D[n].actual));
						//printf("%s->",B[m].data);
					}
				}
				j++; 
				if(strcmp(str1,"MEND") == 0)
					d_flag = 0;
				MDT_CNT++;
			}
			else if((r = Search(str1,A,i)) >= 0) {
				par_CNT = 0;
				token = strtok(NULL,delim2);
				while(token != NULL){
					par_CNT++;
					D[par_CNT-1].positional = par_CNT;
					strcpy(D[par_CNT-1].actual,token);
					token = strtok(NULL,delim2);
				}
			}
			else{		
				fprintf(fp2,"%s",str3);
			}
		}
	}
	for(n = 0; n < par_CNT; n++){
		for(m = 0; m < j; m++){
			itoa(C[n].positional,tmp,10);
			//printf("%s",strrpc(B[m].data,C[n].formal,tmp));
			strcpy(B[m].data,strrpc(B[m].data,C[n].formal,tmp));
			//printf("%s->",B[m].data);
		}
	}
	Print_MNT(A,i);
	Print_MDT(B,j);
	Print_FP(C,k);
	Print_PA(D,par_CNT);
fclose(fp1);
fclose(fp2);
return 0;
}
