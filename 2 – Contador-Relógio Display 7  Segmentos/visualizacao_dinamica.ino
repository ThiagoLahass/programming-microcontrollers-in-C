// C++ code
//
//#include <TimerOne.h>

int pinos[] = {2,3,4,5,6,7,8,9};
//					 a,b,c,d,e,f,g,p
bool disp[10][8] = {{1,1,1,1,1,1,0,1}, //0
					{0,1,1,0,0,0,0,1}, //1
					{1,1,0,1,1,0,1,1}, //2
					{1,1,1,1,0,0,1,1}, //3
					{0,1,1,0,0,1,1,1}, //4
					{1,0,1,1,0,1,1,1}, //5
					{0,0,1,1,1,1,1,1}, //6
					{1,1,1,0,0,0,0,1}, //7
					{1,1,1,1,1,1,1,1}, //8
					{1,1,1,0,0,1,1,1},};//9
int katodo[] = {10,11,12,13};
const int vdin = 5;
int uni = 0;
int dec = 0; 
int cen = 0;
int mil = 0;
int timeVD = 0;

int *pt_k;

void Display7(int *p, int *pk){
	for(int i=0;i<8;i++){
		pinMode(*p,OUTPUT);
		digitalWrite(*p,LOW);
         p++;
	}
  	for(int i=0;i<4;i++){
		pinMode(*pk,OUTPUT);
		digitalWrite(*pk,LOW);
        pk++;
	}
}			

void Display7print(int numero, int *p){
	if (numero >= 0 && numero <= 9){
		for(int i=0;i<8;i++){
			digitalWrite(*p,disp[numero][i]);
            p++;
		}
	}
}		

void setup(void){
	Display7(pinos, katodo);
}


void visDiamica(int *p, int uni, int dec, int cen, int mil){
	Display7print(mil,pinos);
  	digitalWrite(*p,LOW);
  	delay(vdin);
   	digitalWrite(*p,HIGH);
	p++;  
    Display7print(cen,pinos);
  	digitalWrite(*p,LOW);
  	delay(vdin);
   	digitalWrite(*p,HIGH);
  	p++;
    Display7print(dec,pinos);
  	digitalWrite(*p,LOW);
  	delay(vdin);
   	digitalWrite(*p,HIGH);
  	p++;
    Display7print(uni,pinos);
  	digitalWrite(*p,LOW);
  	delay(vdin);
   	digitalWrite(*p,HIGH);
  

}
void loop(){
  if (timeVD == 10){	
 	uni++;
 	timeVD = 0;
    if ( uni > 9){
  		uni=0;
  		dec++;
  	} else if ( dec > 9){
  		dec = 0;
   	 cen++;
  	} else if (cen > 9){
  		cen = 0;
    	mil++;
  	} else if (mil > 9){
  		mil = 0;
  	}
  } else{
  	delay(1);
    timeVD++;
    }
  visDiamica(katodo,uni,dec,cen,mil);
}