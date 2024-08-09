#include "mbed.h"

UnbufferedSerial m920(NC,PA_10,19200);
CAN can(PA_11,PA_12,1000000);
BufferedSerial pc(USBTX,USBRX,115200);
DigitalOut a(PA_0);
DigitalOut b(PA_1);
DigitalOut c(PB_4);
DigitalOut d(PB_5);
//
int data[8];
int input[50];
int input_proc[50];
int nodeNUM;
void get_data();
CANMessage msg,msg_node,msg_stop;
    
int flag;
int main()
{
    // can.frequency(1000000);
    can.mode(CAN::Normal);
    msg.id=0x12;
    msg.len=8;
    msg_node.id=0x11;
    msg_node.len=1;
    msg_stop.id=0x10;
    msg_stop.len=1;

    // m920.attach(get_data);
    d=0;
    while (true) {
        if(m920.readable()){
            // d=!d;
            int i=0;
            do{
                m920.read(&input[i],1);
                input_proc[i]=input[i];
                if(input[i]>='0'&&input[i]<='9')input_proc[i]-='0';
                else if(input[i]>='A'&&input[i]<='F') input_proc[i]=input_proc[i]-'A'+10;  
                d=1;
                if(input[i]==':'){flag=i;c=1;}
            }while(input[i++]!='\n');
            d=0;
            c=0;
            nodeNUM=input_proc[5]*16+input_proc[6];
            msg_node.data[0]=(char)nodeNUM;
            can.write(msg_node);

            if(input_proc[flag+1]==1||input_proc[flag+1]==2){
                msg_stop.data[0]=(char)input_proc[flag+1];//停止
                can.write(msg_stop);
            }else{
                for(int j=0;j<8;j++){
                    data[j]=input_proc[j*2+flag+2]*16+input_proc[j*2+flag+3];//データ変換
                }


                for(int l=0;l<8;l++){
                    msg.data[l]=(char)data[l];
                }
                
                // printf("sousin\r\n");
                if(can.write(msg))a=!a;
            }
            
        }

        b=!b;

    }
    

}

/*
void get_data(){
    if(m920.readable()){
            
            // for(int i=0;i<28;i++){
            //     m920.read(&input[i],1);//データ受信
            // }
            int i=0;
            do{
                m920.read(&input[i],1);
                input_proc[i]=input[i];
                if(input[i]>='0'&&input[i]<='9')input_proc[i]-='0';
                else if(input[i]>='A'&&input[i]<='F') input_proc[i]=input_proc[i]-'A'+10;  
                d=1;
            }while(input[i++]!='Z');
            d=0;
            
            // for(int k=3;k<26;k++){        
            //     if(input[k]>='0'&&input[k]<='9')input[k]-='0';
            //     else if(input[k]>='A'&&input[k]<='F') input[k]=input[k]-'A'+10;    
            // }

            for(int j=0;j<6;j++){
                data[j]=input_proc[j*2+12]*16+input_proc[j*2+13];//データ変換
            }
            data[6]=input_proc[24];
            data[7]=input_proc[25];

            for(int l=0;l<8;l++){
                msg.data[l]=(char)data[l];
            }
            flag=true;
            // if(can.write(msg))a=!a;
            // printf("sousin\r\n");
            
        }
}

*/
