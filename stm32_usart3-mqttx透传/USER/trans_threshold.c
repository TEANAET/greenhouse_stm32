#include "trans_threshold.h"

/*
	*data:Ҫת���ɷ�ֵ������
	*ret_min:���صķ�ֵmin����
	*ret_max:���صķ�ֵmax����
	note��ret����Ϊ�ַ������ָ��
*/

void transTothreshold(char *data, int *ret_min,int *ret_max){
	// �����и�õ���Ӧ��ֵ
						strtok(data, "S*");
						temp = strtok(NULL, "S*");					// eg:�õ�45:60
						ret_min = (int)atoi(strtok(temp, ":"));   //// eg:�õ�45
						ret_max = (u8)atoi(strtok(NULL, ":"));			// // eg:�õ�60	
						u1_printf("�¶���ֵ��%d,%d\r\n",t1,t2);

}

//void T_json(int devicename,int status,char *data, char *ret)
//{
////        char *add1 = "{\"Devicename\":1,\"status\":\"0\",\"msg\":\[";
//				char *add1 = "{\"Devicename\":";
//				char *add3 = ",\"status\":\"";
//				char *add4 = "\",\"msg\":\[";
//        char *add2 = "]}";
//       sprintf(ret,"%s%d%s%d%s%s%s", add1,devicename,add3, status,add4,data,add2);

////        printf("%s",ret);
//}