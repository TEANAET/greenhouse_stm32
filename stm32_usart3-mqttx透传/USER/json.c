#include "json.h"

/*
	*data:Ҫת����json��ʽ������
	*ret:���ص�����
	
	note��ret����Ϊ�ַ������ָ��
*/

void T_json(char *data, char *ret)
{
        char *add1 = "{\"Devicename\":1,\"msg\":\[";
        char *add2 = "]}";
       sprintf(ret,"%s%s%s", add1, data, add2);

//        printf("%s",ret);
}