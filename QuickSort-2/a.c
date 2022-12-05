#include <stdio.h>
#include <string.h>

void quickSortMain(char items[][10], int count);
void quickSort(char items[][10], int left, int right);

int main()
{
  int i,n;
  printf("Enter no. name to sort : ");
  scanf("%d", &n);
  char str[][10] = { "yash","umesh","kharanshu","anand", "sumit", "lokesh"};

  quickSortMain(str, 4);

  for(i=0; i<n; i++) {
     printf("%s ", str[i]);
  }
  return 0;
}

void quickSortMain(char items[][10], int count)
{
  quickSort(items, 0, count-1);
}

void quickSort(char items[][10], int left, int right)
{
  int i, j;
  char *x;
  char temp[10];

  i = left;
  j = right;
  x = items[(left+right)/2];

  do {
    while((strcmp(items[i],x) < 0) && (i < right)) {
       i++;
    }
    while((strcmp(items[j],x) > 0) && (j > left)) {
        j--;
    }
    if(i <= j) {
      strcpy(temp, items[i]);
      strcpy(items[i], items[j]);
      strcpy(items[j], temp);
      i++;
      j--;
   }
  } while(i <= j);

  if(left < j) {
     quickSort(items, left, j);
  }
  if(i < right) {
     quickSort(items, i, right);
  }
}