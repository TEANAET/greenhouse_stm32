#include "json.h"

/*
	*data:Ҫת����json��ʽ������
	*ret:���ص�����
	
	note��ret����Ϊ�ַ������ָ��
*/

void T_json(int devicename,int status,char *data, char *ret)
{
//        char *add1 = "{\"Devicename\":1,\"status\":\"0\",\"msg\":\[";
				char *add1 = "{\"Devicename\":";
				char *add3 = ",\"status\":\"";
				char *add4 = "\",\"msg\":\[";
        char *add2 = "]}";
       sprintf(ret,"%s%d%s%d%s%s%s", add1,devicename,add3, status,add4,data,add2);

//        printf("%s",ret);
}