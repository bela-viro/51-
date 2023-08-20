#include <REGX52.H>
#include <INTRINS.H>
#include "LCD1602.H"
#include "function.h"
#define s_loc 5
#define g_loc 6

unsigned char KeyNum;
unsigned char i;

void main(){
	unsigned int s=0,g=2,cnt=0,isPassWord=1,sumTime = 0,inSetPassWord=1;
	unsigned long InitPassWord=0,InPassWord=0;
	LCD_Init();
	LCD_ShowString(1,1,"PassWord:");
	B:
	while(1){
		Nixie(s_loc,s);Nixie(g_loc,g);
		while(inSetPassWord){//设置密码
			KeyNum = MatrixKey();
			if(KeyNum) 
			{
				if(KeyNum<=10 && cnt<5) 
				{
					InitPassWord *= 10;
					InitPassWord +=KeyNum%10;
					cnt++;
				}
				LCD_ShowNum(2,1,InitPassWord,5);
			}
			if(MatrixKey()==13){
//				LCD_ShowString(1,14,"OK ");
				inSetPassWord = 0;
				break;
			}
		}
		//设置时间
		while(!P3_1){
			Nixie(s_loc,s);Nixie(g_loc,g);
//			LCD_Init();
//			LCD_ShowNum(1,4,m,1);LCD_ShowNum(1,5,s,1);
			Delay(50);
			g++;
			if(g==10){
				s++;
				g=0;
			}else if(s==9 && g==9){
				s = 0;
				g = 0;
			}
		}
		//倒计时
		if(MatrixKey()==16){
//			LCD_ShowString(1,2,"OK");
			while(g!=0 || s!=0){
				Buzzer_Time(100);
				if(s*10+g>10){
					for(sumTime=0; sumTime != 400; sumTime++){//数码管变化时间
						Nixie(s_loc,s);Nixie(g_loc,g);
						Delay(1);
					}
				}else{
					for(sumTime=0; sumTime != 10; sumTime++){//数码管变化时间
						Nixie(s_loc,s);Nixie(g_loc,g);
						Delay(80);
					}
				}
				P2 = 1;Delay(50);
				P2 = 0;Delay(50);
				if(!g){g=10; s--;}
//				LCD_ShowNum(2,4,m,1);LCD_ShowNum(2,5,s,1);
				g--;
				//拆炸弹
				cnt = 0;
				while(!P3_3){
					KeyNum = MatrixKey();
					if(KeyNum) 
					{
						if(KeyNum<=10 && cnt<5) 
						{
							InPassWord *= 10;
							InPassWord +=KeyNum%10;
							cnt++;
						}
					}
					for(i=0; i<7; i++) Nixie(i,0);
				}
				if(InitPassWord == InPassWord){Buzzer_Time_result(1000);goto B;}	
			}
			while(1){
				for(i=1; i<9; i++) Nixie(i,0);
				Buzzer_Time(50);
			}
		}
	}
}
