/*
*******************************************************************************
*  							DATA ACQUISITION SYSTEMS, S.A.
*                             PANAMA, REPUBLIC OF PANAMA
*
*  File 			: FIFO.h
*  Programmer 		: Rangel Alvarado
*  Language			: C/C++
*  Description 		: Header file of FIFO Buffer
*
*  ----------------------------------------------------------------------------
*  HISTORIAL
*  	DD MM AA
*   19 09 16	Created.
*   19 09 16	Modified.
*******************************************************************************
*/

/*
*******************************************************************************
*							MACROS & CONSTANTES
*******************************************************************************
*/

#ifndef MBED_FIFO_H
#define MBED_FIFO_H


#ifndef CFG_H

#define FIFO_TX_MAX	128				/* Max FIFO TXD Chars	*/
#define FIFO_RX_MAX	256				/* Max FIFO RXD Chars	*/

#endif

typedef signed int INT16S;
typedef unsigned char   INT8U;
typedef unsigned char BOOLEAN;
#define TRUE  1
#define FALSE 0

/*
*******************************************************************************
*							MACROS & CONSTANTES
*******************************************************************************
*/


class FIFO {
public:
        FIFO(void);
       ~FIFO();

void    Init(void);
BOOLEAN	RxBufEmpty(void);
BOOLEAN	TxBufFull(void);				/* Fill buffer */
void 	TxPutChar(INT16S data);
INT16S	TxGetChar(void);
void    TxPutLine(INT16S *pdata);
void 	RxPutChar(INT16S data);
INT16S	RxGetChar(void);
void	RxGetLine(INT16S *pdata);
void    RxPutLine(INT16S *pdata);
INT16S  RxGetBytes(void);

private:
    typedef struct fifo_buffer {
	    INT8U	RxBufCtr;				/* Cuenta bytes en el buffer de Tx	*/
	    INT16S	*RxBufHd;				/* Cabeza del buffer de Tx			*/
	    INT16S	*RxBufTl;				/* Cola del buffer de Tx			*/
	    INT16S	RxBuf[FIFO_TX_MAX];	/* Transmission Buffer		*/
	    INT8U	TxBufCtr;				/* Cuenta bytes en el buffer de Rx	*/
	    INT16S	*TxBufHd;				/* Cabeza del buffer de Rx			*/
	    INT16S	*TxBufTl;				/* Cola del buffer de Rx			*/
	    INT16S	TxBuf[FIFO_RX_MAX];	/* Buffer de Recepcion				*/
    } FIFO_BUFFER;
    FIFO_BUFFER fifo;						/* FIFO Buffer Style	*/
};



#endif
