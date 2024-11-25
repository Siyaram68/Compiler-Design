#include <stdio.h>
   #include <stdlib.h>
   #include <ctype.h>
   #include <string.h>
  
  
   #define MAX_PROD 100
   #define MAX_LEN 100
  #define MAX_SYMBOLS 26
 
  typedef struct
  {
      char lhs;
      char rhs[MAX_LEN];
  }production;
 
  production grammar[MAX_PROD];
  int num_productions=0;
  char symbols[MAX_SYMBOLS];
  int num_symbols=0;
  char first_set[MAX_SYMBOLS][MAX_SYMBOLS+1];
  char follow_set[MAX_SYMBOLS][MAX_SYMBOLS+1];
 
  int parse_table[MAX_SYMBOLS][MAX_SYMBOLS];
 
  int add_to_set(char set[],char symbol)
  {
      if(strchr(set,symbol)==NULL)
      {
         int len=strlen(set);
          set[len]=symbol;
          set[len+1]='\0';
          return 1;
      }
      return 0;
  }
 
  int add_set_to_set(char dest[],char src[])
  {
      int changed=0;
      for(int i=0;src[i];i++)
   {
          if(src[i]!='e')
          {
              changed |= add_to_set(dest,src[i]);
          }
      }
      return changed;
  }
 
  int is_not_terminal(char symbol)
  {
      return isupper(symbol);
  }
 
  int symbol_index(char symbol)
  {
      for(int i=0;i<num_symbols;i++)
      {
          if(symbols[i]==symbol)
              return i;
      }
      symbols[num_symbols]=symbol;
      return num_symbols++;
  }
 
  int has_epsilon(char set[])
  {
      return strchr(set,'e')!=NULL;
  }
 
  void compute_first_sets()
  {
      int i,changed;
      do
      {
          changed=0;
          for(int i=0;i<num_productions;i++)
         {
             char lhs=grammar[i].lhs;
             char *rhs=grammar[i].rhs;
            int lhs_index=symbol_index(lhs);

             if(rhs[0]=='e')
             {
                 changed|=add_to_set(first_set[lhs_index],'e');
                 continue;
             }
             int all_epsilon=1;
             for(int j=0;rhs[j];j++)
             {
                 int rhs_index=symbol_index(rhs[j]);
                 if(!is_not_terminal(rhs[j]))
                 {
                     changed|= add_to_set(first_set[lhs_index],rhs[j]);
                     all_epsilon=0;
                     break;
                }
                 else
                 {
                     changed|= add_set_to_set(first_set[lhs_index],first_set[rhs_index]);
                     if (!has_epsilon(first_set[rhs_index]))
                     {
                         all_epsilon=0;
                         break;
                     }
                 }
             }
             if(all_epsilon)
             {
                 changed |=add_to_set(first_set[lhs_index],'e');
             }
         }
     }while (changed);
 }

 void compute_follow_set()
 {
     add_to_set(follow_set[0],'$');
     int changed,i,j,k;
  do
     {
         changed=0;
         for(i=0;i<num_productions;i++)
         {
             char lhs=grammar[i].lhs;
             char *rhs=grammar[i].rhs;
             int lhs_index=symbol_index(lhs);
             for(j=0;rhs[j];j++)
             {
                 if(is_not_terminal(rhs[j]))
                 {
                     int rhs_index=symbol_index(rhs[j]);
                     for(k=j+1;rhs[k];k++)
                     {
                         if(is_not_terminal(rhs[k]))
                         {
                             changed|=add_set_to_set(follow_set[rhs_index],first_set[symbol_index(rhs[k])]);
                             if(!has_epsilon(first_set[symbol_index(rhs[k])]))
                             {
                                 break;
                             }
                         }
                         else
                         {
                             changed |= add_to_set(follow_set[rhs_index],rhs[k]);
                             break;
                         }
                     }
                     if(rhs[k]=='\0' ||(is_not_terminal(rhs[k])) && (has_epsilon(first_set[symbol_index(rhs[k])])))
                     {
                         changed |= add_set_to_set(follow_set[rhs_index],follow_set[lhs_index]);
                     }
                 }
             }
         }
     }while(changed);
 }
void print_sets()
 {
     printf("\nFirst Set\n");
     for(int i=0;i<num_symbols;i++)
     {
         if(is_not_terminal(symbols[i]))
         {
             printf("Fiirst(%c) ={",symbols[i]);
             for(int j=0;first_set[i][j];j++)
             {
                 printf("%c ",first_set[i][j]);
             }
             printf("}\n");
         }
     }
     printf("\nFollow Set\n");
     for(int i=0;i<num_symbols;i++)
     {
         if(is_not_terminal(symbols[i]))
         {
             printf("Follow(%c) ={",symbols[i]);
             for(int j=0;follow_set[i][j];j++)
             {
                 printf("%c ",follow_set[i][j]);
             }
             printf("}\n");
         }
     }
 }

 void print_terminals_variables()
 {
     int first=1,i;
     printf("Variables: [");
     for(int i=0;i<num_symbols;i++)
     {
       if(is_not_terminal(symbols[i]))
         {
             if(!first)
             {
                 printf(",");
             }
             printf("%c",symbols[i]);
             first=0;
         }
     }
     printf("]\n");
     first =1;
     printf("Terminals: [");
     for(int i=0;i<num_symbols;i++)
     {
         if(!is_not_terminal(symbols[i]) && symbols[i]!='e')
         {
             if(!first)
             {
                 printf(", ");
             }
            printf("%c",symbols[i]);
             first=0;
         }
     }
     printf("]\n");
 }

 void print_parse_table()
 {
     printf("\nParsing Table:\n");
     int i,j;
     printf("\t");
     for(i=0;i<num_symbols;i++)
     {
         if(!is_not_terminal(symbols[i])&&symbols[i]!='e')
         {
             printf("%c\t",symbols[i]);
         }
    }
     printf("\n");
     for(i=0;i<num_symbols;i++)
     {
         if(is_not_terminal(symbols[i]))
        {
             printf("%c\t",symbols[i]);
             for(j=0;j<num_symbols;j++)
             {
                if(!is_not_terminal(symbols[j])&&symbols[j]!='e')
                 {
                     if(parse_table[i][j]!=-1)
                     {
                         //printf("%d\t",parse_table[i][j]);
                         int u=parse_table[i][j];
                        printf("%c->%s\t",grammar[u].lhs,grammar[u].rhs);
                     }
                     else
                    {
                         printf("-\t");
                     }
                 }
             }
             printf("\n");
         }
     }
 }

 void build_parse_table()
 {
     int i,j,k;
     for(i=0;i<MAX_SYMBOLS;i++)
     {
         for(j=0;j<MAX_SYMBOLS;j++)
         {
             parse_table[i][j]=-1;
         }
     }
     for(i=0;i<num_productions;i++)
  {
         char lhs=grammar[i].lhs;
         int row = symbol_index(lhs);
         char *rhs=grammar[i].rhs;
         if(rhs[0]=='e')
         {
             for(j=0;follow_set[row][j];j++)
             {
                 int col=symbol_index(follow_set[row][j]);
                 parse_table[row][col]=i;
             }
         }
         else
         {
             for(j=0;rhs[j];j++)
             {
                 char symbol=rhs[j];
                 int col=symbol_index(symbol);
                 if(is_not_terminal(symbol))
                 {
                     if(has_epsilon(first_set[symbol_index(symbol)]))
                    {
                         for(k=0;first_set[symbol_index(symbol)][k];k++)
                         {
                             if(first_set[symbol_index(symbol)][k]!='e')
                             {
                                 parse_table[row][symbol_index(first_set[symbol_index(symbol)][k])]=i;
                             }
                         }
                         for(k=0;follow_set[row][k];k++)
                         {
                             parse_table[row][symbol_index(follow_set[row][k])]=i;
                         }
                     }
                     else
                     {
                         for(k=0;first_set[symbol_index(symbol)][k];k++)
                         {
                             parse_table[row][symbol_index(first_set[symbol_index(symbol)][k])]=i;
  }
                     }
                 }
                 else
                 {
                     parse_table[row][col]=i;
                     break;
                 }
                 break;
             }
         }
     }
 }

 void stack_parsing(char *input)
 {
     char stack[MAX_LEN];
     int top=0;
     stack[top++]='$';
     stack[top++]='S';
     int input_len=strlen(input);
     input[input_len]='$';
     int i=0;
     printf("Stack\t\tInput\t\tAction\n");
     printf("________________________________\n");
     while(top>0)
     {
         printf("%s\t\t%s\t\t",stack,input+i);
         char top_symbol=stack[--top];
         if(top_symbol=='$'&& input[i]=='$')
         {
             printf("Accept\n");
             return;
         }
         if(!is_not_terminal(top_symbol))
         {
             if(top_symbol==input[i])
             {
                 printf("Match '%c'\n",input[i]);
  i++;
             }
             else
             {
                 printf("Error:Unexpected symbol '%c'\n",input[i]);
                 return;
             }
         }
         else
         {
             int row=symbol_index(top_symbol);
             int col=symbol_index(input[i]);
             if(parse_table[row][col]!=-1)
             {
                 int prod_index=parse_table[row][col];
                 char *rhs = grammar[prod_index].rhs;
                 printf("Apply %c -%s\n",grammar[prod_index].lhs,rhs);
                 int j;
                 int rhs_len=strlen(rhs);
                 for(j=rhs_len-1;j>=0;j--)
                 {
                     if(rhs[j]!='e')
                     {
                         stack[top++]=rhs[j];
                     }
                 }
                 stack[top]='\0';
             }
             else
             {
                 printf("String Rejected\n");
                 return;
             }
         }
     }
     printf("Error: Stack empty before input consumed\n");
 }
 int main()
{
     int i,j;
     char input[MAX_LEN];
     printf("Enter number of productions:");
     scanf("%d",&num_productions);
     for(int i=0;i<num_productions;i++)
     {
         printf("Enter productions:\n");
         scanf("%s",input);

         char *arrow=strstr(input,"->");
         if(arrow==NULL)
         {
             exit(1);
         }
         grammar[i].lhs=input[0];
         strcpy(grammar[i].rhs,arrow+2);
         symbol_index(grammar[i].lhs);
         for(j=0;grammar[i].rhs[j];j++)
         {
             symbol_index(grammar[i].rhs[j]);
         }
     }
     print_terminals_variables();
     compute_first_sets();
     compute_follow_set();
     print_sets();
     build_parse_table();
     print_parse_table();
     char inp[MAX_LEN];
     printf("Enter the string to parse:");
     scanf("%s",inp);
     stack_parsing(inp);
     return 0;
     }
     return 0;
     }

