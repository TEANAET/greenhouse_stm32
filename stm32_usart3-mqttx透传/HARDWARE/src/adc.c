 
#include "delay.h"
#include "sys.h"
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3	
int Soil_Temp;
void T_Adc_Init(void)  //ADCͨ����ʼ��
{
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��GPIOA,ADC1ͨ��ʱ��
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //��Ƶ����6ʱ��Ϊ72M/6=12MHz

 	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

//	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	
 
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1);	//����ָ����ADC1�ĸ�λ�Ĵ���

    while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�

	ADC_StartCalibration(ADC1);	 //

	while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
}
u16 T_Get_Adc(u8 ch)   
	{
 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��3,��һ��ת��,����ʱ��Ϊ239.5����	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	}

//�õ�ADC�����ڲ��¶ȴ�������ֵ
//ȡ10��,Ȼ��ƽ��
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

 //��ȡͨ��ch��ת��ֵ
//ȡtimes��,Ȼ��ƽ��
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

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)	//��ȡ�ڲ��¶ȴ������¶�ֵ
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=T_Get_Adc_Average(ADC_Channel_16,20);	//��ȡͨ��16,20��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ 
	temperate=(1.43-temperate)/0.0043+25;	//ת��Ϊ�¶�ֵ 	 
	result=temperate*=100;					//����100��.
	return result;
}

//��ʼ��ADC3
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��6																	   
void  Adc3_Init(void)
{      
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3	, ENABLE );	  //ʹ��ADC3ͨ��ʱ��
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);//ADC��λ
	
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);//��λ����	    
	
	ADC_DeInit(ADC3);  //��λADC3,������ ADC3��ȫ���Ĵ�������Ϊȱʡֵ
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ: ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC3, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
	
	
	ADC_Cmd(ADC3, ENABLE);	//ʹ��ָ����ADC3
	
	ADC_ResetCalibration(ADC3);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC3));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC3);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC3));	 //�ȴ�У׼����
}		 
//���ADC3ĳ��ͨ����ֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc3(u8 ch)   
{
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC3,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//ʹ��ָ����ADC3�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC3);	//�������һ��ADC3�������ת�����
} 

void Adc_Soil_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef		ADC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
/*PF_3����Ϊģ������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����
	ADC_DeInit(ADC1);//��λADC
	//��ʼ��ADC����
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1,ENABLE);//ʹ��ADC1
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}
 
u16  Get_Soil_Adc(u8 ch)  //��ȡADC����
{
	ADC_RegularChannelConfig(ADC1,ch,1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}
 

u16 Get_Soil_Adc_Average(u8 ch,u8 times)  //��ȡ����ʪ�����ݲ����ظ�������
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
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12M  ���14M ����ADCʱ�ӣ�ADCCLK��
	ADC_DeInit(ADC3);
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//ADC
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
 
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 4; 
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);
	
	//�ڲ��¶ȴ���������ADC1ͨ��16�ġ�
	ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
//	ADC_TempSensorVrefintCmd(ENABLE);//���ڲ��¶ȴ�����ʹ��
	ADC_Cmd(ADC3,ENABLE);	
 
	ADC_ResetCalibration(ADC3);//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC3));//��ȡADC����У׼�Ĵ�����״̬
	
	ADC_StartCalibration(ADC3);//��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC3));//��ȡָ��ADC��У׼����
 
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
	GPIO_SetBits(GPIOC, GPIO_Pin_2);//��1
}


 
u16 GetGP2YSingleValue(void)
{
		int samplingTime = 280;//�ȴ�LED������ʱ����280��s
//		int deltaTime = 40;//�����������ʱ��Ϊ320��s����ˣ����ǻ����ٵȴ�40��s
	
		uint16_t ADCVal;
		float dustVal = 0;
		float Voltage;
	
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);//��1  �����ڲ�LED  ????????
		DelayUs(samplingTime); 					// ����LED���280us�ĵȴ�ʱ��
		
		ADC_SoftwareStartConvCmd(ADC3, ENABLE);
    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
		ADCVal = ADC_GetConversionValue(ADC3);
	
		DelayUs(40);
		GPIO_SetBits(GPIOC, GPIO_Pin_2);//��0
		DelayUs(9680);//��Ҫ�����0.32ms/10ms��PWM�ź������������е�LED
		Voltage = ADCVal * 3.3 / 4096;
		dustVal = (Voltage + 1) * 1000 / 10;//*1000�����ǽ���λת��Ϊug/m3
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
