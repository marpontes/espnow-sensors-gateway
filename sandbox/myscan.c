/** 
 * parsing messages
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool startsWith(const char *pre, const char *str){
    return strncmp(pre, str, strlen(pre)) == 0;
}

bool startsWith2(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int main(){
    char format[] = "rgbw_123,131,31,1110";
    char format2[] = "rgbw_123,131,31,1110";
    char* ptr1 = strtok(format, "_");
    char* prefix = ptr1;
    ptr1 = strtok(NULL, "_");
    char* values = ptr1;
    printf("prefix: '%s'\n", prefix);
    printf("values: '%s'\n", values);
    char* ptr = strtok(values, ",");
    int i = 0;
    int rgbw[4];

    while (ptr) {
      rgbw[i] = atoi(ptr);
      printf("'%d' \n", rgbw[i]);
      ptr = strtok(NULL, ",");
      i++;
    }
    char rgbwPrefix[] = "rgbw_";
    if(startsWith(rgbwPrefix, format)){
      printf("OK\n" );
    }else{
      printf("Nao OK\n");
      printf("%d", strncmp(rgbwPrefix, format2, strlen(rgbwPrefix)));
    }
}
