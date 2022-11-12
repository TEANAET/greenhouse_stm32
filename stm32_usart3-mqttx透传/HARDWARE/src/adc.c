 
#include "delay.h"
#include "sys.h"
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3	
int Soil_Temp;
void T_Adc_Init(void)  //ADC通道初始化
{
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能GPIOA,ADC1通道时钟
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //分频因子6时钟为72M/6=12MHz

 	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

//	ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
	
 
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	ADC_ResetCalibration(ADC1);	//重置指定的ADC1的复位寄存器

    while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待

	ADC_StartCalibration(ADC1);	 //

	while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
}
u16 T_Get_Adc(u8 ch)   
	{
 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,第一个转换,采样时间为239.5周期	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	}

//得到ADC采样内部温度传感器的值
//取10次,然后平均
u16 T_Get_Temp(void)
	{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<10;t++)
		{
		temp_val+=T_Get_Adc(ADC_Channel_16);	  //TampSensor
		DelayMs(5);
		}
	return temp_val/10;
	}

 //获取通道ch的转换值
//取times次,然后平均
u16 T_Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=T_Get_Adc(ch);
		DelayMs(5);
	}
	return temp_val/times;
} 	   

//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
short Get_Temprate(void)	//获取内部温度传感器温度值
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=T_Get_Adc_Average(ADC_Channel_16,20);	//读取通道16,20次取平均
	temperate=(float)adcx*(3.3/4096);		//电压值 
	temperate=(1.43-temperate)/0.0043+25;	//转换为温度值 	 
	result=temperate*=100;					//扩大100倍.
	return result;
}

//初始化ADC3
//这里我们仅以规则通道为例
//我们默认仅开启通道6																	   
void  Adc3_Init(void)
{      
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3	, ENABLE );	  //使能ADC3通道时钟
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);//ADC复位
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);//复位结束	    
	
	ADC_DeInit(ADC3);  //复位ADC3,将外设 ADC3的全部寄存器重设为缺省值
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式: 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC3, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
	
	
	ADC_Cmd(ADC3, ENABLE);	//使能指定的ADC3
	
	ADC_ResetCalibration(ADC3);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC3));	//等待复位校准结束
	
	ADC_StartCalibration(ADC3);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC3));	 //等待校准结束
}		 
//获得ADC3某个通道的值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc3(u8 ch)   
{
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC3,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//使能指定的ADC3的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC3);	//返回最近一次ADC3规则组的转换结果
} 

void Adc_Soil_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef		ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
/*PF_3设置为模拟输入*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC分频因子
	ADC_DeInit(ADC1);//复位ADC
	//初始化ADC参数
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1,ENABLE);//使能ADC1
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}
 
u16  Get_Soil_Adc(u8 ch)  //获取ADC数据
{
	ADC_RegularChannelConfig(ADC1,ch,1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}
 

u16 Get_Soil_Adc_Average(u8 ch,u8 times)  //获取土壤湿度数据并返回给主函数
{
	u32 temp_val=0;
	
	float temp_avrg;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Soil_Adc(ch);
		DelayMs(1);
	}
	temp_avrg=temp_val/times;
	Soil_Temp=(4092-temp_avrg)/3292*100;
	return Soil_Temp;
}







void GP2Y_ADC_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_AFIO|RCC_APB2Periph_ADC3,ENABLE);
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12M  最大14M 设置ADC时钟（ADCCLK）
	ADC_DeInit(ADC3);
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//ADC
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
 
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 4; 
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);
	
	//内部温度传感器是在ADC1通道16的。
	ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
//	ADC_TempSensorVrefintCmd(ENABLE);//打开内部温度传感器使能
	ADC_Cmd(ADC3,ENABLE);	
 
	ADC_ResetCalibration(ADC3);//重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC3));//获取ADC重置校准寄存器的状态
	
	ADC_StartCalibration(ADC3);//开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC3));//获取指定ADC的校准程序
 
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);//使能或者失能指定的ADC的软件转换启动功能
	GPIO_SetBits(GPIOC, GPIO_Pin_2);//置1
}


 
u16 GetGP2YSingleValue(void)
{
		int samplingTime = 280;//等待LED开启的时间是280μs
//		int deltaTime = 40;//整个脉冲持续时间为320μs。因此，我们还需再等待40μs
	
		uint16_t ADCVal;
		float dustVal = 0;
		float Voltage;
	
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);//置1  开启内部LED  ????????
		DelayUs(samplingTime); 					// 开启LED后的280us的等待时间
		
		ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
		ADCVal = ADC_GetConversionValue(ADC3);
	
		DelayUs(40);
		GPIO_SetBits(GPIOC, GPIO_Pin_2);//置0
		DelayUs(9680);//需要脉宽比0.32ms/10ms的PWM信号驱动传感器中的LED
		Voltage = ADCVal * 3.3 / 4096;
		dustVal = (Voltage + 1) * 1000 / 10;//*1000作用是将单位转换为ug/m3
//		if(ADCVal > 36.455)
//			dustVal = ((float)(ADCVal / 1024.0) - 0.0356) * 120000 * 0.035;
//		dustVal = 0.17 * ADCVal - 0.1;
//		dustVal = ADCVal;
//		return dustVal; //		mg/m3
		return dustVal;
}

u16 GetGP2YAverageValue(u8 times)
{
    u32 temp_val=0;
    u8 t;
    for(t=0;t<times;t++)
    {
        temp_val+=GetGP2YSingleValue();
    }
    return temp_val/times;
}
