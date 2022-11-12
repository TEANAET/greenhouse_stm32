#include "stm32f10x.h"
#include "MQ_Sensor.h"
#include <stdio.h>

	u16 ADC_Val = 0;
/**************************** ADCģ���ʼ������ ****************************
��  �ܣ���ʼADC_1ģ��
��  ������
����ֵ����
Ӳ�����ӣ�
			MQ2����ȼ���壩   ---> ADC1_IN2��AO�� --->PA2
***************************************************************************/
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* ����GPIO�ں�ADC��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
	/* ����ADCģ�鹤��Ƶ�� 72/6 = 12M */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	/* ����ADCͨ���ܽŶ�Ӧ��GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                          //ģ����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/* ����ADCģ�鹤��ģʽ */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                      //ADC����ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     //����ѭ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           //��ͨ��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     //���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  //�����Ҷ��� -- ��12λ
	ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //˳��ת���Ĺ������ͨ����Ŀ
	ADC_Init(ADC1,&ADC_InitStructure);                                      //����ؼĴ�����ֵ
	
	/* ʹ��ADCģ�� */
	ADC_Cmd(ADC1,ENABLE);                                                   //����ADC1
	
	/* ADCģ��У�� */
	ADC_ResetCalibration(ADC1);						//����ADC1��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));		//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�

	ADC_StartCalibration(ADC1);					    //��ʼADC1��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));		    //�ȴ�У׼���
}

/**************************** ADCת��ֵ��ȡ���� ****************************
��  �ܣ���ȡADCģ��ת�������ֵ
��  ������
����ֵ�����ADCת��ֵ
***************************************************************************/
u16 Get_adcvalue(void)
{
	u16 Buff[10] = {0};
	u16 temp = 0;
	u8 i,j;
	
	ADC_RegularChannelConfig(ADC1,2,1,ADC_SampleTime_239Cycles5);   //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	
	for(i = 0; i < 10; i++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);               //�������ADCת��
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET)
		{
			/* �ȴ�ת����� */
		}
		Buff[i] = ADC_GetConversionValue(ADC1);             //��ȡADC���ݼĴ����е�ֵ
	}
	/* ADC����˲� */     /*ð���㷨*/
	for(i = 0; i < 9; i++)
	{
		for(j = i + 1; j < 10; j++)
		{
			if(Buff[i] > Buff[j])
			{
				temp = Buff[i];
				Buff[i] = Buff[j];
				Buff[j] = temp;
			}
		}
	}
	temp = 0;
	for(i = 1; i < 9; i++)
	{
		temp += Buff[i];
	}
	return (temp / 8);
}

/*************************** ADCת�������ʾ���� **************************
�������ܣ�ADC���ս������
������
	Page����ʾ����ĺ�����
	Colum����ʾ�����������
����ֵ����
**************************************************************************/
void ADC_Val_Disp(u8 Page,u8 Colum)
{
	u16 ADC_Temp = 0;                     //ADCת�����������
	//float ADC_TT = 0;
//	char Buff[20];
	
	/* ��ȡADCת��ֵ */
	ADC_Temp = Get_adcvalue();        //ģ����=�ο���/ADCλ��*������     ����300-5000ppm  ��:
	ADC_Val = (u16)((((5000.0 - 300) / 4096) * ADC_Temp) + 300);			
//	sprintf(Buff,"gas:%dppm",ADC_Val);
//	printf("\r\nGas:%dppm\r\n",ADC_Val);
	/* Һ������ʾ */
//	LCD_Dis_Str_Sf(Page,Colum,Buff,BLACK,WHITE,2);
//	LCD_Dis_String(Page,Colum,Buff,RGB(0,0,0),RGB(255,255,255),1);
}
