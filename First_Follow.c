#include <ctype.h>
   #include <stdio.h>
   #include <string.h>
  
   int count, n = 0;
   char calc_first[50][100];
   char calc_follow[50][100];
  int m = 0;
 
  char prods[50][50];
  char f[50],first[50];
 
 
  void findfirst(char c,int q1,int q2){
      if(!(isupper(c))) first[n++] = c;
      for(int j=0;j<count;j++){
          if(prods[j][0] == c){
              //printf("\n%c: %c (%ld)\n",c,prods[j][3],strlen(prods[j]));
              if(prods[j][3] == '#'){
                  //printf("ptr: %c\n",prods[q1][q2]);
                 if(prods[q1][q2] == '\0') first[n++] = '#';
                  else if(prods[q1][q2]!='\0' && (q1!=0 || q2!=0)) findfirst(prods[q1][q2], q1, (q2+1));
                  else first[n++] = '#';
              }
              else if(!isupper(prods[j][3])) first[n++]=prods[j][3];
              else{
                  if(q2!=strlen(prods[j])) findfirst(prods[j][3], j, 3);
                  else return;
              }
          }
      }
 }
 
  void followfirst(char c, int q1, int q2);
  void follow(char c){
      if(prods[0][0]==c) f[m++]='$';
      for(int i=0;i<20;i++){
          for(int j=3;j<20;j++){
              if(prods[i][j]==c){
                  if(prods[i][j+1]!='\0') followfirst(prods[i][j+1], i, j+3);
                  if(prods[i][j+1]=='\0' && c!=prods[i][0]) follow(prods[i][0]);
           }
          }
      }
  }
 
  void followfirst(char c, int q1, int q2){
      if(!(isupper(c))) f[m++] = c;
      else{
          int i=0, j=1;
          for(i=0;i<count;i++){
              if(calc_first[i][0]==c) break;
          }
 
          while(calc_first[i][j]!='!'){
              if(calc_first[i][j]!='#') f[m++]=calc_first[i][j];
              else{
                  if(prods[q1][q2]=='\0') follow(prods[q1][0]);
                  else followfirst(prods[q1][q2], q1, q2+1);
              }
              j++;
          }
      }
  }
 
  int main(){
      int x = 0;
      int choice;
      char c;
 
      printf("Enter the number of productions: ");
      scanf("%d",&count);
      getchar();
      for(int i=0;i<count;i++){
          //printf("Enter production %d: ",i+1);
          fgets(prods[i],sizeof(prods[i]),stdin);
          prods[i][strcspn(prods[i],"\n")]='\0';
      }
     
      char done[count];
      int ptr=-1;
      for(int k = 0; k < count; k++) {
          for(int len=0;len<100;len++) calc_first[k][len] = '!';
      }
     int point1 = 0, point2, flag;
    for (int k = 0; k < count; k++) {
         c=prods[k][0];
         point2=0;
         flag=0;

         for(int len=0;len<=ptr;len++){
             if(c == done[len]) flag=1;
         }

         if(flag==1) continue;

         findfirst(c, 0, 0);
         ptr+=1;

         done[ptr] = c;
         printf("\n First(%c) = { ", c);
         calc_first[point1][point2++] = c;

         for(int i=0+x;i<n;i++){
             int chk=0;
             for(int pos=0;pos<point2;pos++) {
                 if(first[i] == calc_first[point1][pos]){
                     chk = 1;
                     break;
                 }
             }
             if(chk == 0){
                 printf("%c ", first[i]);
                 calc_first[point1][point2++] = first[i];
            }
         }
         printf("}\n");
         x = n;
         point1++;
    }
     printf("\n--------------------------------------------------------\n");
     
     //follow
     char donee[count];
     ptr=-1;
     x=0;
    point1=0;
    int land = 0;
     for(int k=0;k<count;k++){
         for(int len=0;len<100;len++) calc_follow[k][len]='!';
     }

     for(int k=0;k<count;k++){
         c=prods[k][0];
         point2=0;
         flag=0;

         for(int len=0;len<=ptr;len++)
             if(c==donee[len]) flag=1;

         if(flag==1) continue;
         
         land+=1;
         follow(c);
         ptr+=1;

         donee[ptr]=c;
         printf(" Follow(%c) = { ", c);
         calc_follow[point1][point2++] = c;

         for(int i=0+x; i<m; i++) {
             int chk = 0;
             for(int pos=0; pos<point2; pos++){
                 if(f[i] == calc_follow[point1][pos]){
                     chk = 1;
                     break;
                 }
             }
            if(chk == 0){
                 printf("%c ", f[i]);
                 calc_follow[point1][point2++] = f[i];
             }
         }
         printf(" }\n\n");
         x = m;
         point1++;
     }
 }

