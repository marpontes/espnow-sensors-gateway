/** 
 * Simple demonstration of dumping formatted values
 * into a string. 
 * Showcasing for messaging purposes
*/
#include <stdio.h>
#include <string.h>

int main(){
    char * buffer;
    char format[] = "{\"t\":%.2f, \"h\":%.2f, \"hi\":%.2f}";
    asprintf(&buffer, format, 1.1, 1.2, 3.3);
    puts(buffer);
}
