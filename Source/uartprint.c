#include "Configure.h"

void  DelayNS(INT32U dly) {  
  INT32U i;
  for(; dly>0; dly--) 
    for(i=0; i<100; i++);
}

void uart_sendbyte(INT8U  data)
{
  U1THR  = data;
  while((U1LSR&0x40 )==0);
  DelayNS(1000);
}

void uart_sendstring( INT8U  *  dpata)
{
  while(	*dpata != '\0')
  {
    uart_sendbyte(*dpata);
    dpata++;
  }
}

void debug(INT8U * data){
#if DEBUG==TRUE
  uart_sendstring(data);
  uart_sendstring("\n\r");
#endif
}

void debug_INT8U(INT8U data){
#if DEBUG==TRUE
  if(data==(INT8U)0x0){
    uart_sendbyte('0');
  }
  while(data){
    uart_sendbyte(data%10+'0');
    data /=10;
  }
  uart_sendstring("\n\r");
#endif
}

void debug_INT32U(INT32U data){
#if DEBUG==TRUE
  if(data==(INT32U)0x0){
    uart_sendbyte('0');
  }
  while(data){
    uart_sendbyte(data%10+'0');
    data /=10;
  }
  uart_sendstring("\n\r");
#endif
}

INT8U uart_rev(void)
{
  INT8U rcv_data;
  while(( U1LSR  & 0X01)  == 0) ;
  rcv_data = U1RBR;
  return     rcv_data;
}

