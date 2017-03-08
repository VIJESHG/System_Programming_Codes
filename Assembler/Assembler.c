#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
int main(int argc,char *argv[]) {
	FILE *fp1,*fp2,*fp3,*fp4,*fp5;
	int LC =-1,found,endfound,count;
	char address[50],length[20];
	char str[128],mnemonic[50],type[10],*token,str1[50],str2[50],str3[50],str4[50],str5[50];
	char delim1[2] = ";",delim2[3] = " 	\n";
	char type1[4] = "ID",type2[4] = "var",type3[5] = "const",blank[3] = "--";	
	fp1 = fopen("MOT.txt","r");
	fp2 = fopen(argv[1],"r");
	fp3 = fopen("ST.txt","w+");
	fp4 = fopen("Intermediate.txt","w+");
	fp5 = fopen("Output.txt","w+");
	if(fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL ) {
		perror("open failed! ");
		return errno;
	}
	while(fgets(str,128,fp2)){						/* read file line by line */
		token = strtok(str,delim2);					/* get the first word of each line */
		strcpy(str1,token);							/* copy the first word for future use */
		fseek(fp1,0,SEEK_SET);						/* jump at starting of MOT table */
		while(1) {									/* read until first maching word does'nt found */	
			fscanf(fp1,"%s%s",mnemonic,address);
			if(feof(fp1)) {							/* break when EOF */
				break;
			}	
			if(strcmp(token,mnemonic) == 0) {		/* cmp word with each mnemonic in the MOT */
				endfound = 1;						/* flag if END found */
				if(strcmp(mnemonic,"END") != 0){	/* check if end is found or not */
					endfound = 0;					/* if end is not found  increment LC*/
					LC++;							
				}
				fprintf(fp4,"%d		%s\n",LC,address);     /* print into Intermediate file */
				printf("%d: %s	%s\n",LC,mnemonic,address);
			}
			
		}
		while(token != NULL && endfound == 0) {			/* tokenizing line first word onwards */
			token = strtok(NULL,delim2);				
			if(token != NULL) {
				fseek(fp1,0,SEEK_SET);
				found = 0;
				while(1) {								/* scan the word into MOT */
					fscanf(fp1,"%s%s",mnemonic,address);		
					if(feof(fp1)) {						/* break if EOF */
						if(found == 0) {				/* if not found */
							LC++;	
							printf("%s ->",token);						
							fprintf(fp3,"%s		%s		%s\n",token,type1,blank);	/* add the entry to ST with blank address */
							fprintf(fp4,"%d		%s\n",LC,token);					/* write Lc and token name in Intermediate file*/
							printf("%d: %s	%s\n",LC,token,token);			
						}
						break;
					}	
					if(strcmp(token,mnemonic) == 0) {			/* entry found in MOT then add to Intermediate file*/
						found = 1;
						LC++;
						//fprintf(fp3,"%s		%s",mnemonic,type1);
						fprintf(fp4,"%d		%s\n",LC,address);
						//printf("%d: %s	%s\n",LC,mnemonic,address);
					}
			
				}
			}
		}
		while(token != NULL && endfound == 1) {				/* END of CODE segment */
			token = strtok(NULL,delim2);					
			if(token != NULL) {
				fseek(fp1,0,SEEK_SET);					
				found = 0;
				while(1) {
					fscanf(fp1,"%s%s",mnemonic,address);
					if(feof(fp1)) {
						if(found == 0) {
							LC++;
							fseek(fp3,0,SEEK_SET);
							count = -1;
							while(1) {
								fscanf(fp3,"%s",str2);
								if(feof(fp3))
									break;
								count++;
								if(count % 3 == 0) {
									if(strcmp(str2,str1) == 0) {	
										fprintf(fp4,"%d		%s\n",LC,token);								
										fprintf(fp3,"		%s",type1);
										count++;
										fprintf(fp3,"		%s",token);
										count++;	
									}
								}
							}
							break;
						}	
					}
					if(found == 1) 
						break;	
				}
			}
		}
	}
	fseek(fp3,0,SEEK_SET);
	fseek(fp4,0,SEEK_SET);
	while(1) {
		found = 0;
		fscanf(fp4,"%s%s",str1,str2);
		if(feof(fp4)){
			break;
		}
		fseek(fp3,0,SEEK_SET);
		while(1){
			fscanf(fp3,"%s%s%s",str3,str4,str5);
			if(feof(fp3)){
				break;
			}
			if(strcmp(str2,str3) == 0 && found != 1){
				fprintf(fp5,"%s		%s\n",str1,str5);
				found = 1;
			}
		}
		if(found == 0){
			fprintf(fp5,"%s		%s\n",str1,str2);
		}
	}			
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
return 0;
}	
