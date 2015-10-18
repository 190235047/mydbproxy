#include <stdio.h>  
#include "mysql.h"  
  
MYSQL my_connection;  
MYSQL_RES *res_ptr;  
MYSQL_ROW sqlrow;  
int main(int argc, char *argv[])
{
   mysql_init(&my_connection);    
   if (mysql_real_connect(&my_connection, "10.10.38.135", "root", "123456", NULL, 0, NULL, 0)) {  
       printf("Connection successaaa\n");
   } else {
       printf("Connection error\n");
if(mysql_errno(&my_connection)) {
	printf("Connection error %d: %s\n", mysql_errno(&my_connection), mysql_errno(&my_connection));
} 
 }
while(1) {}
}
