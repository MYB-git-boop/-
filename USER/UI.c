
#include "UI.h"

unsigned short int Keytimes=0;				//按键按下时间计数
unsigned char Kaysta=0;								//按键状态变量
unsigned char WorkMode=0;							//工作模式变量
unsigned int Voltage=0;									//电池电压变量，单位0.1V
char Menux=0;													//菜单变量
unsigned char UI_Display_Flag=0;			//标记界面写入一次
//short int	VoltLowWarn=0;							//低电压报警值
//unsigned short int VoltLowTimes=0;		//电压低检测次数
short int PS2_CH[3];
unsigned char Control_Flag=0;		

unsigned char Title_Strings[6][13]={
"[ 功能选择 ]\0",	"[ 手柄模式 ]\0",	"[ 蓝牙模式 ]\0","[ 寻迹模式 ]\0","[ 避障模式 ]\0","[	同步模式 ]\0",
};

unsigned char StandbyUI_String[3][6][7]={
"前左：\0","前右：\0","后左：\0","后右：\0","电压：\0","距离：\0",
"前左：\0","前右：\0","後左：\0","後右：\0","電壓：\0","距离：\0",
"F_L： \0","F_R： \0","B_L： \0","B_R： \0","Volt：\0","Dis：\0",
};
unsigned char StandbyUI_String2[2][21]={
" <短按切换,长按启动>\0",
"<  短按退出  >\0",
};

unsigned char SettingUI_String[5][11]={
"手柄模式  \0","蓝牙模式  \0","寻迹模式  \0","避障模式  \0","同步模式  \0",
};

u8 Standby_Mode(void)
{
	static u8 flag=1;
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		OLED_Put12x12CNstr(6,13,SettingUI_String[0],LIGHT);			OLED_Put12x12CNstr(75,13,SettingUI_String[1],LIGHT);
		OLED_Put12x12CNstr(6,26,SettingUI_String[2],LIGHT);			OLED_Put12x12CNstr(75,26,SettingUI_String[3],LIGHT);	
		OLED_Put12x12CNstr(6,39,SettingUI_String[4],LIGHT);	
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[0],LIGHT);			//显示提示语
	
		UI_Display_Flag=0;
	}
	if(Kaysta==1)				//按键短按
	{
	 	Menux++;	//模式0，Standby
		if(Menux==6) Menux=1;
		UI_Display_Flag=1;	//标记改变界面
		Kaysta=0;	//清除按键标记
										//清除按键标记
	}
	if(Kaysta==2)	//按键长按
	{
		switch(Menux)
		{
			case 1:WorkMode=1;break;
			case 2:WorkMode=2;break;
			case 3:WorkMode=3;break;
			case 4:WorkMode=4;break;
			case 5:WorkMode=5;break;
			default:break;
	
		}
		UI_Display_Flag=1;						//标记改变界面
		Kaysta=0;											//清除按键标记
		flag=0;
	}
	Display_Menux(Menux);	
	OLED_Refresh_AllGDRAM(); 	//刷新显示
	return flag;																				
}	

void Ps2_Mode(void)
{
		Uln_Trig();			
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			OLED_Put12x12CNstr(66,39,StandbyUI_String[0][5],LIGHT);	
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
		ARMED=1;
		UI_Display_Flag=0;	
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
	OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
	OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
	OLED_Put6x12Num(98,42,Distance1,5,1);			//声波距离

}

void Blutooth_Mode(void)
{

	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
	OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
	OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
}

void Tracking_Mode(void)
{

	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}		
	OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
	OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
}

void Avoid_Mode(void)
{
	Uln_Trig();						//发送超声波
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			OLED_Put12x12CNstr(66,39,StandbyUI_String[0][5],LIGHT);	

		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
		OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
		OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
		OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
		OLED_Put6x12Num(98,42,Distance1,5,1);			//声波
}
void Line_Mode(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			OLED_Put12x12CNstr(66,39,StandbyUI_String[0][5],LIGHT);	

		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
		ARMED=1;
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
		OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
		OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
		OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
}



void Display_Menux(char Menuxx)
{
	if(Menuxx==1)	OLED_Put5x7Char(0,15,'>',LIGHT);
	else 					OLED_Put5x7Char(0,15,' ',LIGHT);
	if(Menuxx==2)	OLED_Put5x7Char(66,15,'>',LIGHT);
	else					OLED_Put5x7Char(66,15,' ',LIGHT);
	if(Menuxx==3)	OLED_Put5x7Char(0,28,'>',LIGHT);
	else					OLED_Put5x7Char(0,28,' ',LIGHT);
	if(Menuxx==4)	OLED_Put5x7Char(66,28,'>',LIGHT);
	else					OLED_Put5x7Char(66,28,' ',LIGHT);
	if(Menuxx==5)	OLED_Put5x7Char(0,41,'>',LIGHT);
	else					OLED_Put5x7Char(0,41,' ',LIGHT);
	
}




	
