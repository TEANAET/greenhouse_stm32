#include "trans_threshold.h"

/*
	*data:要转换成阀值的数据
	*ret_min:返回的阀值min数据
	*ret_max:返回的阀值max数据
	note：ret必须为字符数组的指针
*/

void transTothreshold(char *data, int *ret_min,int *ret_max){
	// 进行切割得到对应阈值
						strtok(data, "S*");
						temp = strtok(NULL, "S*");					// eg:得到45:60
						ret_min = (int)atoi(strtok(temp, ":"));   //// eg:得到45
						ret_max = (u8)atoi(strtok(NULL, ":"));			// // eg:得到60	
						u1_printf("温度阈值：%d,%d\r\n",t1,t2);

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