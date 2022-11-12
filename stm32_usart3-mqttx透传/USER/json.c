#include "json.h"

/*
	*data:要转换成json格式的数据
	*ret:返回的数据
	
	note：ret必须为字符数组的指针
*/

void T_json(char *data, char *ret)
{
        char *add1 = "{\"Devicename\":1,\"msg\":\[";
        char *add2 = "]}";
       sprintf(ret,"%s%s%s", add1, data, add2);

//        printf("%s",ret);
}