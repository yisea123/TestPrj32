#include "ethernetif.h" 
#include "lan8720.h"  
#include "lwip_comm.h" 
#include "netif/etharp.h"  
#include "string.h"  

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ethernetif.c 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/8/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

 
////由ethernetif_init()调用用于初始化硬件
////netif:网卡结构体指针 
////返回值:ERR_OK,正常
////       其他,失败
//static err_t low_level_init(struct netif *netif)
//{
//#ifdef CHECKSUM_BY_HARDWARE
//	int i; 
//#endif 
//	netif->hwaddr_len = ETHARP_HWADDR_LEN; //设置MAC地址长度,为6个字节
//	//初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复
//	netif->hwaddr[0]=lwipdev.mac[0]; 
//	netif->hwaddr[1]=lwipdev.mac[1]; 
//	netif->hwaddr[2]=lwipdev.mac[2];
//	netif->hwaddr[3]=lwipdev.mac[3];
//	netif->hwaddr[4]=lwipdev.mac[4];
//	netif->hwaddr[5]=lwipdev.mac[5];
//	netif->mtu=1500; //最大允许传输单元,允许该网卡广播和ARP功能
//	//并且该网卡允许有硬件链路连接
//	netif->flags = NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;
//	
//	ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); //向STM32F4的MAC地址寄存器中写入MAC地址
//	ETH_DMATxDescChainInit(DMATxDscrTab, Tx_Buff, ETH_TXBUFNB);
//	ETH_DMARxDescChainInit(DMARxDscrTab, Rx_Buff, ETH_RXBUFNB);
//#ifdef CHECKSUM_BY_HARDWARE 	//使用硬件帧校验
//	for(i=0;i<ETH_TXBUFNB;i++)	//使能TCP,UDP和ICMP的发送帧校验,TCP,UDP和ICMP的接收帧校验在DMA中配置了
//	{
//		ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
//	}
//#endif
//	ETH_Start(); //开启MAC和DMA				
//	return ERR_OK;
//} 
////用于发送数据包的最底层函数(lwip通过netif->linkoutput指向该函数)
////netif:网卡结构体指针
////p:pbuf数据结构体指针
////返回值:ERR_OK,发送正常
////       ERR_MEM,发送失败
//static err_t low_level_output(struct netif *netif, struct pbuf *p)
//{
//	u8 res;
//	struct pbuf *q;
//	int l = 0;
//	u8 *buffer=(u8 *)ETH_GetCurrentTxBuffer(); 
//	for(q=p;q!=NULL;q=q->next) 
//	{
//		memcpy((u8_t*)&buffer[l], q->payload, q->len);
//		l=l+q->len;
//	} 
//	res=ETH_Tx_Packet(l); 
//	if(res==ETH_ERROR)return ERR_MEM;//返回错误状态
//	return ERR_OK;
//} 
////用于接收数据包的最底层函数
////neitif:网卡结构体指针
////返回值:pbuf数据结构体指针
//static struct pbuf * low_level_input(struct netif *netif)
//{  
//	struct pbuf *p, *q;
//	u16_t len;
//	int l =0;
//	FrameTypeDef frame;
//	u8 *buffer;
//	p = NULL;
//	frame=ETH_Rx_Packet();
//	len=frame.length;//得到包大小
//	buffer=(u8 *)frame.buffer;//得到包数据地址 
//	p=pbuf_alloc(PBUF_RAW,len,PBUF_POOL);//pbufs内存池分配pbuf
//	if(p!=NULL)
//	{
//		for(q=p;q!=NULL;q=q->next)
//		{
//			memcpy((u8_t*)q->payload,(u8_t*)&buffer[l], q->len);
//			l=l+q->len;
//		}    
//	}
//	frame.descriptor->Status=ETH_DMARxDesc_OWN;//设置Rx描述符OWN位,buffer重归ETH DMA 
//	if((ETH->DMASR&ETH_DMASR_RBUS)!=(u32)RESET)//当Rx Buffer不可用位(RBUS)被设置的时候,重置它.恢复传输
//	{ 
//		ETH->DMASR=ETH_DMASR_RBUS;//重置ETH DMA RBUS位 
//		ETH->DMARPDR=0;//恢复DMA接收
//	}
//	return p;
//} 
////网卡接收数据(lwip直接调用)
////netif:网卡结构体指针
////返回值:ERR_OK,发送正常
////       ERR_MEM,发送失败
//err_t ethernetif_input(struct netif *netif)
//{
//	err_t err;
//	struct pbuf *p;
//	p=low_level_input(netif);
//	if(p==NULL) 
//		return ERR_MEM;
//	err=netif->input(p, netif);
//	if(err!=ERR_OK)//逻辑有问题，如果不等于就不释放p了
//	{
//		LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_input: IP input error\n"));
//		pbuf_free(p);
//		p = NULL;
//	} 
//	return err;
//} 
////使用low_level_init()函数来初始化网络
////netif:网卡结构体指针
////返回值:ERR_OK,正常
////       其他,失败
//err_t ethernetif_init(struct netif *netif)
//{
//	LWIP_ASSERT("netif!=NULL",(netif!=NULL));
//#if LWIP_NETIF_HOSTNAME			//LWIP_NETIF_HOSTNAME 
//	netif->hostname="lwip";  	//初始化名称
//#endif 
//	netif->name[0]=IFNAME0; 	//初始化变量netif的name字段
//	netif->name[1]=IFNAME1; 	//在文件外定义这里不用关心具体值
//	netif->output=etharp_output;//IP层发送数据包函数
//	netif->linkoutput=low_level_output;//ARP模块发送数据包函数
//	low_level_init(netif); 		//底层硬件初始化函数
//	return ERR_OK;
//}


/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////

 //static struct netif *s_pxNetIf = NULL;
/**
* This function should do the actual transmission of the packet. The packet is
* contained in the pbuf that is passed to the function. This pbuf
* might be chained.
*
* @param netif the lwip network interface structure for this ethernetif
* @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
* @return ERR_OK if the packet could be sent
*         an err_t value if the packet couldn't be sent
*
* @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
*       strange results. You might consider waiting for space in the DMA queue
*       to become availale since the stack doesn't retry to send a packet
*       dropped because of memory failure (except for the TCP timers).
*/

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  u8 *buffer ;
  __IO ETH_DMADESCTypeDef *DmaTxDesc;
  uint16_t framelength = 0;
  uint32_t bufferoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t payloadoffset = 0;

  
  
    DmaTxDesc = DMATxDescToSet;
    buffer = (u8 *)(DmaTxDesc->Buffer1Addr);
    bufferoffset = 0;

    for(q = p; q != NULL; q = q->next) 
    {
      if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
      {
        goto error;
      }

      /* Get bytes in current lwIP buffer  */
      byteslefttocopy = q->len;
      payloadoffset = 0;

      /* Check if the length of data to copy is bigger than Tx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
      {
        /* Copy data to Tx buffer*/
        memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

        /* Point to next descriptor */
        DmaTxDesc = (ETH_DMADESCTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

        /* Check if the buffer is available */
        if((DmaTxDesc->Status & ETH_DMATxDesc_OWN) != (u32)RESET)
        {
          goto error;
        }

        buffer = (u8 *)(DmaTxDesc->Buffer1Addr);

        byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
        framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      /* Copy the remaining bytes */
      memcpy( (u8_t*)((u8_t*)buffer + bufferoffset), (u8_t*)((u8_t*)q->payload + payloadoffset), byteslefttocopy );
      bufferoffset = bufferoffset + byteslefttocopy;
      framelength = framelength + byteslefttocopy;
    }

    /* Prepare transmit descriptors to give to DMA*/
    ETH_Prepare_Transmit_Descriptors(framelength);

    /* Give semaphore and exit */
  error:

  

  return ERR_OK;
}
/**
* Should allocate a pbuf and transfer the bytes of the incoming
* packet from the interface into the pbuf.
*
* @param netif the lwip network interface structure for this ethernetif
* @return a pbuf filled with the received packet (including MAC header)
*         NULL on memory error
*/
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p= NULL, *q;
  u32_t len;
  FrameTypeDef frame;
  u8 *buffer;
  __IO ETH_DMADESCTypeDef *DMARxDesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i=0;  

  /* get received frame */
  frame = ETH_Get_Received_Frame_interrupt();

  /* Obtain the size of the packet and put it into the "len" variable. */
  len = frame.length;
  buffer = (u8 *)frame.buffer;
  
  if (len > 0)
  {
    /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  }
  
  if (p != NULL)
  {
    DMARxDesc = frame.descriptor;
    bufferoffset = 0;
    for(q = p; q != NULL; q = q->next)
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;

      /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        /* Copy data to pbuf*/
        memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

        /* Point to next descriptor */
        DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
        buffer = (unsigned char *)(DMARxDesc->Buffer1Addr);

        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      /* Copy remaining data in pbuf */
      memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), byteslefttocopy);
      bufferoffset = bufferoffset + byteslefttocopy;
    }
  }
  
  /* Release descriptors to DMA */
  DMARxDesc =frame.descriptor;

  /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
  for (i=0; i<DMA_RX_FRAME_infos->Seg_Count; i++)
  {  
    DMARxDesc->Status = ETH_DMARxDesc_OWN;
    DMARxDesc = (ETH_DMADESCTypeDef *)(DMARxDesc->Buffer2NextDescAddr);
  }
  
  /* Clear Segment_Count */
  DMA_RX_FRAME_infos->Seg_Count =0;
  
  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET)  
  {
    /* Clear RBUS ETHERNET DMA flag */
    ETH->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    ETH->DMARPDR = 0;
  }
  return p;
}
/**
* This function is the ethernetif_input task, it is processed when a packet 
* is ready to be read from the interface. It uses the function low_level_input() 
* that should handle the actual reception of bytes from the network
* interface. Then the type of the received packet is determined and
* the appropriate input function is called.
*
* @param netif the lwip network interface structure for this ethernetif
*/
err_t ethernetif_input( struct netif *netif )//返回值无意义
{
  struct pbuf *p;
  err_t err;
TRY_GET_NEXT_FRAME:
	p = low_level_input( netif );
	if   (p != NULL)
	{
		if (ERR_OK != (err = netif->input( p, netif)))
		{
			pbuf_free(p);
		}
		else
		{
			goto TRY_GET_NEXT_FRAME;
		}
	}
	return err;
}

/**
* In this function, the hardware should be initialized.
* Called from ethernetif_init().
*
* @param netif the already initialized lwip network interface structure
*        for this ethernetif
*/
static void low_level_init(struct netif *netif)
{
  uint32_t i;

  /* set netif MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set netif MAC hardware address */
  netif->hwaddr[0] = lwipdev.mac[0]; 
	netif->hwaddr[1] = lwipdev.mac[1]; 
	netif->hwaddr[2] = lwipdev.mac[2];
	netif->hwaddr[3] = lwipdev.mac[3];
	netif->hwaddr[4] = lwipdev.mac[4];
	netif->hwaddr[5] = lwipdev.mac[5];

  /* set netif maximum transfer unit */
  netif->mtu = 1500;

  /* Accept broadcast address and ARP traffic */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  //s_pxNetIf =netif;


  /* initialize MAC address in ethernet MAC */ 
  ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); 

  /* Initialize Tx Descriptors list: Chain Mode */
  ETH_DMATxDescChainInit(DMATxDscrTab, Tx_Buff, ETH_TXBUFNB);
  /* Initialize Rx Descriptors list: Chain Mode  */
  ETH_DMARxDescChainInit(DMARxDscrTab, Rx_Buff, ETH_RXBUFNB);

  /* Enable Ethernet Rx interrrupt *///需要使用ETH_Get_Received_Frame_interrupt
  { 
    for(i=0; i<ETH_RXBUFNB; i++)
    {
      ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);
    }
  }

#ifdef CHECKSUM_BY_HARDWARE
  /* Enable the checksum insertion for the Tx frames */
  {
    for(i=0; i<ETH_TXBUFNB; i++)
    {
      ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
    }
  } 
#endif

    /* Enable MAC and DMA transmission and reception */
    ETH_Start();   
}
/**
* Should be called at the beginning of the program to set up the
* network interface. It calls the function low_level_init() to do the
* actual setup of the hardware.
*
* This function should be passed as a parameter to netif_add().
*
* @param netif the lwip network interface structure for this ethernetif
* @return ERR_OK if the loopif is initialized
*         ERR_MEM if private data couldn't be allocated
*         any other err_t on error
*/
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

//  etharp_init();
//  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);

  return ERR_OK;
}











