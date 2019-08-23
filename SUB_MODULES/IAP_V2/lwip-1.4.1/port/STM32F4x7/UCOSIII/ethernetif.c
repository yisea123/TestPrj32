#include "ethernetif.h" 
#include "lan8720.h"  
#include "lwip_comm.h" 
#include "netif/etharp.h"  
#include "string.h"  

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ethernetif.c ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

 
////��ethernetif_init()�������ڳ�ʼ��Ӳ��
////netif:�����ṹ��ָ�� 
////����ֵ:ERR_OK,����
////       ����,ʧ��
//static err_t low_level_init(struct netif *netif)
//{
//#ifdef CHECKSUM_BY_HARDWARE
//	int i; 
//#endif 
//	netif->hwaddr_len = ETHARP_HWADDR_LEN; //����MAC��ַ����,Ϊ6���ֽ�
//	//��ʼ��MAC��ַ,����ʲô��ַ���û��Լ�����,���ǲ����������������豸MAC��ַ�ظ�
//	netif->hwaddr[0]=lwipdev.mac[0]; 
//	netif->hwaddr[1]=lwipdev.mac[1]; 
//	netif->hwaddr[2]=lwipdev.mac[2];
//	netif->hwaddr[3]=lwipdev.mac[3];
//	netif->hwaddr[4]=lwipdev.mac[4];
//	netif->hwaddr[5]=lwipdev.mac[5];
//	netif->mtu=1500; //��������䵥Ԫ,����������㲥��ARP����
//	//���Ҹ�����������Ӳ����·����
//	netif->flags = NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;
//	
//	ETH_MACAddressConfig(ETH_MAC_Address0, netif->hwaddr); //��STM32F4��MAC��ַ�Ĵ�����д��MAC��ַ
//	ETH_DMATxDescChainInit(DMATxDscrTab, Tx_Buff, ETH_TXBUFNB);
//	ETH_DMARxDescChainInit(DMARxDscrTab, Rx_Buff, ETH_RXBUFNB);
//#ifdef CHECKSUM_BY_HARDWARE 	//ʹ��Ӳ��֡У��
//	for(i=0;i<ETH_TXBUFNB;i++)	//ʹ��TCP,UDP��ICMP�ķ���֡У��,TCP,UDP��ICMP�Ľ���֡У����DMA��������
//	{
//		ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
//	}
//#endif
//	ETH_Start(); //����MAC��DMA				
//	return ERR_OK;
//} 
////���ڷ������ݰ�����ײ㺯��(lwipͨ��netif->linkoutputָ��ú���)
////netif:�����ṹ��ָ��
////p:pbuf���ݽṹ��ָ��
////����ֵ:ERR_OK,��������
////       ERR_MEM,����ʧ��
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
//	if(res==ETH_ERROR)return ERR_MEM;//���ش���״̬
//	return ERR_OK;
//} 
////���ڽ������ݰ�����ײ㺯��
////neitif:�����ṹ��ָ��
////����ֵ:pbuf���ݽṹ��ָ��
//static struct pbuf * low_level_input(struct netif *netif)
//{  
//	struct pbuf *p, *q;
//	u16_t len;
//	int l =0;
//	FrameTypeDef frame;
//	u8 *buffer;
//	p = NULL;
//	frame=ETH_Rx_Packet();
//	len=frame.length;//�õ�����С
//	buffer=(u8 *)frame.buffer;//�õ������ݵ�ַ 
//	p=pbuf_alloc(PBUF_RAW,len,PBUF_POOL);//pbufs�ڴ�ط���pbuf
//	if(p!=NULL)
//	{
//		for(q=p;q!=NULL;q=q->next)
//		{
//			memcpy((u8_t*)q->payload,(u8_t*)&buffer[l], q->len);
//			l=l+q->len;
//		}    
//	}
//	frame.descriptor->Status=ETH_DMARxDesc_OWN;//����Rx������OWNλ,buffer�ع�ETH DMA 
//	if((ETH->DMASR&ETH_DMASR_RBUS)!=(u32)RESET)//��Rx Buffer������λ(RBUS)�����õ�ʱ��,������.�ָ�����
//	{ 
//		ETH->DMASR=ETH_DMASR_RBUS;//����ETH DMA RBUSλ 
//		ETH->DMARPDR=0;//�ָ�DMA����
//	}
//	return p;
//} 
////������������(lwipֱ�ӵ���)
////netif:�����ṹ��ָ��
////����ֵ:ERR_OK,��������
////       ERR_MEM,����ʧ��
//err_t ethernetif_input(struct netif *netif)
//{
//	err_t err;
//	struct pbuf *p;
//	p=low_level_input(netif);
//	if(p==NULL) 
//		return ERR_MEM;
//	err=netif->input(p, netif);
//	if(err!=ERR_OK)//�߼������⣬��������ھͲ��ͷ�p��
//	{
//		LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_input: IP input error\n"));
//		pbuf_free(p);
//		p = NULL;
//	} 
//	return err;
//} 
////ʹ��low_level_init()��������ʼ������
////netif:�����ṹ��ָ��
////����ֵ:ERR_OK,����
////       ����,ʧ��
//err_t ethernetif_init(struct netif *netif)
//{
//	LWIP_ASSERT("netif!=NULL",(netif!=NULL));
//#if LWIP_NETIF_HOSTNAME			//LWIP_NETIF_HOSTNAME 
//	netif->hostname="lwip";  	//��ʼ������
//#endif 
//	netif->name[0]=IFNAME0; 	//��ʼ������netif��name�ֶ�
//	netif->name[1]=IFNAME1; 	//���ļ��ⶨ�����ﲻ�ù��ľ���ֵ
//	netif->output=etharp_output;//IP�㷢�����ݰ�����
//	netif->linkoutput=low_level_output;//ARPģ�鷢�����ݰ�����
//	low_level_init(netif); 		//�ײ�Ӳ����ʼ������
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
err_t ethernetif_input( struct netif *netif )//����ֵ������
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

  /* Enable Ethernet Rx interrrupt *///��Ҫʹ��ETH_Get_Received_Frame_interrupt
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











