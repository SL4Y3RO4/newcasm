#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum type_t {
	INT,
	CHR,
	STR,
	BOOL
} Type;


int slen(char *s) {
	int count = 0;
	while(*s) {
		count++;
		s++;
	}
	return count;
}

int is_number(char c) {
	return c >= '0' && c <= '9';
}

int is_letter(char c) {
	return c >= 'a' && c <= 'z';
}

int is_operator(char c) {
	return c == '<' || c == '>' || c == '=' || c == '!' 
	    || c == '+' || c == '-' || c == '*' || c == '/';
}

int is_specialchar(char c) {
	return c == '_' || c == '@' || c == '#' || c == '-' || c == '.';
}

int is_upCase(char c) {
	return c >= 'A' && c <= 'Z';
}


char *conc(char *a, char *b) {
	
	char* temp = malloc((slen(a) + slen(b)) + 1);
	int k = 0;
	
	while(*a) {
		temp[k++] = *a;
		a++;
	}
	
	k = 0;
	while(*b) {
		temp[k++] = *b;
		b++;
	}
	temp[k] = '\0';
	return temp;
}


void sub(char *s, FILE* f, char* argname, int val) {
	rewind(f);
	
	char *varnames[554];
	int namec = 0;
	int numsc[254];
	int numc = 0;
	char *strlist[554];
	int scroll = 0;
	Type type[554];
	int typec = 0;
	int count_inter = 0;
	int flags[254];
	int flagc = 0;
	char *boolList[254];
	int booln = 0;
	char takechr[254];
	int chrc = 0;
	
		
	while(fgets(s, 254, f) != NULL) {
		
		s[strcspn(s, "\n")] = '\0';
		
		while(*s == ' ' || *s == '\0' || *s == '\t' || *s == '=') s++;
		
		//int
		if(strncmp(s, "int", 3) == 0) {
			count_inter++;
			flags[flagc++] = 1;
			type[typec++] = INT;
			s = s + 3;
			int *nums = malloc(slen(s) + 1); 
			int k = 0, j = 0, i = 0;
			char op;
			int count_i = 0;
			int is_var = 0;
			char *varname = malloc(slen(s) + 1);
			char *decname = malloc(slen(s) + 1);
			while(*s == ' ') s++;
			
			while(*s != '=') {
				if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
					varname[k++] = *s;
				}
				s++;
			}
			varname[k] = '\0';
		    varnames[namec++] = varname;
			
			if(*s == '=') {
				s++;
				while(*s != ';') {
					
					if(is_operator(*s)) {
						op = *s;
					}
					
					if(is_number(*s)) {
						nums[j++] = atoi(s);
						is_var = 0;
						count_i++;
					} else if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
						decname[j++] = *s;
						is_var = 1;
					}
					s++;
				}
			}
		    
		    if(is_var) {
		    	decname[j] = '\0';
		    	for(int i = 0; i < namec; i++) {
				    if(strcmp(decname, argname) == 0) {
				       numsc[numc++] = val;
				    }
			    }
			} else {
			  numsc[numc++] = nums[0];	
			}
		 
		 	
			
		 for(int i = 1; i < count_i; i++) {
		      switch(op) {
		      	case '+': numsc[0] += nums[i]; break;
		      	case '-': numsc[0] -= nums[i]; break;
		      	case '/': numsc[0] /= nums[i]; break;
		      	case '*': numsc[0] *= nums[i]; break;
			  }
		 }
			
			
		  //string
		} else if(strncmp(s, "string", 6) == 0) {
		  type[typec++] = STR;
		  flags[flagc++] = 3;
          s = s + 6;
          int k = 0, j = 0;
		  char *varname = malloc(slen(s) + 1);
		  char *string = malloc(slen(s) + 1);
          while(*s == ' ') s++;
			
		  while(*s != '=') {
				if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
					varname[k++] = *s;
				}
				s++;
			}
			varname[k] = '\0';
		    varnames[namec++] = varname;
		    
		    if(*s == '=') {
		    	s++;
		    	while(*s == ' ') s++;
		    	if(*s == '"') {
		    		s++;
		    		while(*s != '"') {
		    			string[j++] = *s;
		    		s++;
					}
				}
				string[j] = '\0';
				strlist[scroll++] = string;
			}
			
			
	      //bool
	    } else if(strncmp(s, "bool", 4) == 0) {
	    	type[typec++] = BOOL;
			s = s + 4;
			int k = 0, j = 0;
			char *varname = malloc(slen(s) + 1);
			char *boolc = malloc(slen(s) + 1);
			while(*s == ' ') s++;
			
			   while(*s != '=') {
			   	  if(is_number(*s) || is_letter(*s) || is_specialchar(*s) || is_upCase(*s)) {
			   	  	  varname[k++] = *s;
				  }
				  s++;
			   }
			   varname[k] = '\0';
			   varnames[namec++] = varname;
			
			    
			   while(*s != ';') {
			   	    if(is_number(*s) || is_letter(*s) || is_specialchar(*s) || is_upCase(*s)) {
			   	    	boolc[j++] = *s;
					}
					s++;
			   }
			   boolc[j] = '\0';
			   
			   if(strcmp(boolc, "True") == 0) {
			   	   boolList[booln++] = boolc;
			   } else if(strcmp(boolc, "False") == 0) {
			   	   boolList[booln++] = boolc;
			   } else {
			   	   boolList[booln++] = "null";
			   }
			   
		  //char  
	    } else if(strncmp(s, "char", 4) == 0) {
	    	type[typec++] = CHR;
	    	s = s + 4;
	    	int k = 0;
	    	char getchr;
	    	char *varname = malloc(slen(s) + 1);
	    	while(*s == ' ') s++;
	    	
	    	while(*s != '=') {
	    		if(is_number(*s) || is_letter(*s) || is_specialchar(*s) || is_upCase(*s)) {
	    			varname[k++] = *s;
				}
	    		s++;
			}
			varname[k] = '\0';
			varnames[namec++] = varname;
			
			if(*s == '=') {
				s++;	
				while(*s != ';') {
					if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
						getchr = *s;
					}
					s++;
				}
				takechr[chrc++] = getchr;
			}
			
	    	
	      //op	
		} else if(strncmp(s, "op", 2) == 0) {
	    	s = s + 2;
	    	int k = 0, j = 0, i = 0, l = 0;
	    	char *varname = malloc(slen(s) + 1);
	    	char *string = malloc(slen(s) + 1);
	    	int *nums = malloc(slen(s) + 1);
	    	int check_pos = 0;
	    	int is_numeric = 0;
	    	int is_string = 0;
	    	char *decname = malloc(slen(s) + 1);
	    	int is_var = 0;
	        int cmp = 0;
	    	
		    while(*s == ' ') s++;
		    
		    while(*s != ',') {
		       if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
		       		varname[k++] = *s;
			   }
		       s++;
		    }
		    varname[k] = '\0';
			
		if(*s == ',') {
			s++;
			while(*s == ' ') s++;
			if(*s == '"') {
				s++;
				while(*s != '"') {
					string[j++] = *s;
					s++;
				}
				string[j] = '\0';
				is_string = 1;
				
			} else {
				while(*s != ';') {
					if(is_number(*s)) {
						nums[i++] = atoi(s);
					} else if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
						decname[l++] = *s;
					}
					s++;
				}
				is_numeric = 1;
				decname[l] = '\0';
				varnames[namec++] = decname; 
				
				if(decname != NULL) {
					is_numeric = 0;
					is_string = 0;
					for(int i = 0; i < namec; i++) {
                       if(strcmp(varnames[i], decname) == 0) {
                       	  	is_var = 1;
                       	  	cmp = i;
					   }						
					} //search template
					
				}
			}
			
		}
			for(int i = 0; i < namec; i++) {		
				if(strcmp(varnames[i], varname) == 0 && (is_string && !is_numeric)) {	
					strcat(strlist[0], string);
					
				} else if(strcmp(varnames[i], varname) == 0 && (!is_string && is_numeric)) {
					char temp[2];
					sprintf(temp, "%d", nums[0]);
					strcat(strlist[0], temp);
					
				} else if(strcmp(varnames[i], varname) == 0 && (is_var && flags[i] == 1)) {
					char temp[2];
					sprintf(temp, "%d", numsc[0]);
					strcat(strlist[0], temp);
					
				} else if(strcmp(varnames[i], varname) == 0 && (is_var && flags[i] == 3)) {
					strcat(strlist[0], strlist[1]);
				}
			}
		    
		    
		  //print
		} else if(strncmp(s, "print", 5) == 0) {
			s = s + 5;
			int k = 0, j = 0;
			char *varname = malloc(slen(s) + 1);
			int check_pos = 0;
			while(*s == ' ') s++;
			
			while(*s != ';') {
				if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
					varname[k++] = *s;
				}
				s++;
			}
			varname[k] = '\0';
			
		
			for(int i = 0; i < namec; i++) {
				if(strcmp(varnames[i], varname) == 0) {
					switch(type[i]) {
						case INT: printf("%d", numsc[i]); break;
						case STR: printf("%s", strlist[i]); break;
						case BOOL: printf("%s", boolList[i]); break;
						case CHR: printf("%c", takechr[i]); break;
					}
						
				}
			}
			
	    	
	      //syscall
	    } else if(strncmp(s, "syscall", 7) == 0) {
	    	s = s + 7;
	    	char *varname = malloc(slen(s) + 1);
	    	int i = 0, k = 0, j = 0;
	    	int check_pos = 0;
	    	char *string = malloc(slen(s) + 1);
	    	while(*s == ' ') s++;
	    	
	    	if(*s == '"') {
	    		s++;
	    		while(*s != '"') {
	    			string[j++] = *s;
	    			s++;
				}
				string[j] = '\0';
				
			} else {
			   	while(*s != ';') {
				    if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
					   varname[k++] = *s;
				    } 
				    s++;
			    }	
				varname[k] = '\0';
			}
	    
			for(int i = 0; i < namec; i++) {
				if(strcmp(varnames[i], varname) == 0) {
					check_pos = i;
					system(strlist[check_pos]);
				} 
			}
			
			if(string != NULL) {
				system(string);
			}
	    	
	   }	
   }   
}


int main() {
	
	FILE *f = fopen("file.txt", "r");
	char line[254];
	
	
	while(fgets(line, 254, f) != NULL) {
		
		line[strcspn(line, "\n")] = '\0';
		
		char *s = line;
		
		while(*s == ' ' || *s == '\0' || *s == '\t' || *s == '=') s++;
		
		
		//for
		if(strncmp(s, "for", 3) == 0) {
			s = s + 3;
			int k = 0, j = 0, i = 0, l = 0;
			char op;
			int *nums = malloc(slen(s) + 1);
			int *max_num = malloc(slen(s) + 1);
			int *iter_val = malloc(slen(s) + 1);
			char *varname = malloc(slen(s) + 1);
			char op2 = '\0';
			while(*s == ' ') s++;
			
		    if(*s == '(') {
		    	s++;
		    	while(*s != ':') {
		    		//int
		    		if(strncmp(s, "int", 3) == 0) {
		    			s = s + 3;
		    			
		    			while(*s != '=') {	
		    				if(is_number(*s) || is_letter(*s) || is_specialchar(*s)) {
		    					varname[j++] = *s;
							}
							s++;
						}
						varname[j] = '\0';
						
						if(*s == '=') {
							s++;
							while(*s != ';') {
							  if(is_number(*s)) {
							  	 nums[k++] = atoi(s);
							  }
								s++;
							}
						}
						
						if(*s == ';') {
							s++;
							while(*s != ';') {
								
								if(is_operator(*s)) {
									op = *s;
								}
								
								if(is_number(*s)) {
									max_num[i++] = atoi(s);
								}
								s++;
							}
						}
						
						if(*s == ';') {
							s++;
							while(*s != ')') {
								
								if(is_operator(*s)) {
									op2 = *s;
								}
								
								if(is_number(*s)) {
									iter_val[l++] = atoi(s);
								}
								s++;
							}
						}		
		    			   
					} //end int
		    		s++;
				}
				if(*s == ':') {
				    s++;
					switch(op) {
						case '<': for(int i = nums[0]; i < max_num[0]; i+=iter_val[0]) { sub(s, f, varname, i); } break;
					}
				}
				
		    } else {
		    	
		    	while(*s != ':') {
		    		
		    		if(is_operator(*s)) {
		    			op = *s;
					}
		    		
		    		if(is_number(*s)) {
		    			nums[k++] = atoi(s);
					}
		    		s++;
				}
				
				if(*s == ':') {
				    s++;
					switch(op) {
						case '<': for(int i = nums[0]; i < nums[1]; i++) { sub(s, f, "", 0); } break;
					}
				}
				
			}
			
		}
		
		
	}
	fclose(f);
}
			

	  