/*
*******************************************************************************
*  							DATA ACQUISITION SYSTEMS, S.A.
*                             PANAMA, REPUBLIC OF PANAMA
*
*  File 			: FIFO.c
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
#include "FIFO.h"


/*
*******************************************************************************
*
*  					INITIALIZES FIFO BUFFER (CONSTRUCTOR)
*
*  Descripción  	: Head and Tail at the start position.
*  Argumentos   	: None
*  Retorno			: None
*  Notas    		: Buffer empty
*******************************************************************************
*/

FIFO::FIFO() {
	fifo.TxBufCtr = 0;				/* Contador del buffer Tx vacío		*/
	fifo.TxBufHd  = fifo.TxBufTl = &fifo.TxBuf[0];
									/* Cabeza = Cola = Buf Tx en Inicio	*/
	fifo.RxBufHd  = fifo.RxBufTl = &fifo.RxBuf[0];
									/* Cabeza = Cola = Buf Rx en Inicio	*/
	fifo.RxBufCtr = 0;				/* Contador del buffer Rx vacío		*/
}

FIFO::~FIFO()
{
    delete &fifo;
}

void FIFO::Init(void)
{
//    FIFO::FIFO();
}

/*
*******************************************************************************
*
*  					VERIFICA SI EL BUFFER DE RECEPCIÓN ESTÁ VACÍO
*
*  Descripción  	: Verifica si existen datos en el buffer de Rx.
*  Argumentos   	: Ninguno
*  Retorno			: El estado del buffer vacío o con nueva data
*  Notas    		: TRUE		si el buffer de recepción está vacío
*					  FALSE		si el buffer de recepción no está vacío
*******************************************************************************
*/

BOOLEAN	FIFO::RxBufEmpty(void)
{									/* Estado de buffer vacío			*/
	BOOLEAN status;					/* variable de estado				*/


//	DisableInterrupts;				/* sei (CCR_I = 1)					*/
	status = FALSE;					/* Asume que no está vacío			*/
	if (fifo.RxBufCtr == 0)	/* ¿Contador de bytes es cero?		*/
		status =  TRUE;				/* SI, Buffer vacío					*/
//	EnableInterrupts;				/* cli (CCR_I = 0)					*/
	return (status);				/* retorna el estado del buffer		*/
}

/*
*******************************************************************************
*
*  				VERIFICA SI EL BUFFER DE TRANSMISIÓN ESTÁ LLENO
*
*  Descripción  	: Verifica si se agotó el espacio del buffer de Tx.
*  Argumentos   	: Ninguno
*  Retorno			: Capacidad del buffer de transmisión agotada o no
*  Notas    		: TRUE		si el buffer de transmisión está lleno
*					  FALSE		si el buffer de transmisión no está lleno
*******************************************************************************
*/

BOOLEAN	FIFO::TxBufFull(void)
{									/* Estado de buffer lleno			*/
	BOOLEAN status;					/* variable de estado				*/


//	DisableInterrupts;				/* sei (CCR_I = 1)					*/
	status = TRUE;					/* estado inicial					*/
	if (fifo.TxBufCtr < FIFO_TX_MAX)
									/* ¿Buffer Lleno?					*/
		status = FALSE;				/* FALSE = Con espacio en el buffer	*/
//	EnableInterrupts;				/* cli (CCR_I = 0)					*/
	return (status);				/* retorna el estado del buffer		*/
}

/*
*******************************************************************************
*
*  						IMPONE UN BYTE EN EL BUFFER DE TRANSMISIÓN
*
*  Descripción  	: Mueve 1 byte al buffer de transmisión.
*  Argumentos   	: data		es el byte a agregar al buffer de transmisión.
*  Retorno			: Ninguno
*  Notas    		: Al primer byte agregado, habilita la interrupción de Tx
*					  Si el buffer está lleno, el caracter es ignorado
*******************************************************************************
*/

void 	FIFO::TxPutChar(INT16S data)
{
	if (fifo.TxBufCtr < FIFO_TX_MAX) {
		fifo.TxBufCtr++;
		*fifo.TxBufHd++ = data;
		if (fifo.TxBufHd == &fifo.TxBuf[FIFO_TX_MAX]) {
			fifo.TxBufHd = &fifo.TxBuf[0];
        }
		if (fifo.TxBufCtr == 1) {
		    ;
		}
	}
}

/*
*******************************************************************************
*
*  						REMUEVE UN BYTE DEL BUFFER DE TRANSMISIÓN
*
*  Descripción  	: Remueve 1 byte del buffer de transmisión.  Instalar esta
*					  función en la subrutina de interrupción junto a
*					  SCITxPut().
*  Argumentos   	: Ninguno
*  Retorno			: Caracter extraido del buffer de Tx, a enviar por el SCI.
*  Notas    		: Si el buffer está vacío retorna un valor nulo (0)
*******************************************************************************
*/

INT16S	FIFO::TxGetChar(void)
{									/* Recoge 1 byte del buffer			*/
	INT16S temp;						/* Variable temporal				*/


	temp = 0;						/* valor inicial					*/
	if (fifo.TxBufCtr > 0) {	/* Si el buffer no está vacío		*/
		fifo.TxBufCtr--;		/* Decremento contador				*/
		if (fifo.TxBufCtr == 0) {
			//SCITxIntDis();			/* Deshabilita interrupción del Tx	*/
		}
		temp = *fifo.TxBufTl++;
									/* recojo el dato del buffer e in-
									   cremento a la siguiente posición	*/
		if (fifo.TxBufTl == &fifo.TxBuf[FIFO_TX_MAX])
									/* Si la cola llega al final		*/
			fifo.TxBufTl = &fifo.TxBuf[0];
									/* Apunto a la primera posición		*/
	}
	return (temp);					/* Retorna el dato si pudo extraer
									   sino, un valor nulo				*/
}

/*
*******************************************************************************
*
*  		ENVÍA UNA LÍNEA DE CARACTERES AL SERIAL POR EL PUERTO SERIAL
*
*  Descripción  	: Envía una cadena por el puerto serial.
*  Argumentos   	: pdata		es la posición inicial de la trama a enviar
*  Retorno			: Ninguno
*  Notas    		: Ninguna
*******************************************************************************
*/

void FIFO::TxPutLine(INT16S *pdata)
{									/* Envía una trama de bytes			*/
	INT16S data;						/* Variable temporal del caracter 	*/


	data = 0;						/* Inicializa data en 0				*/
	while (*pdata != '\0') {		/* Mientras el caracter a enviar
									   sea diferente al final de cadena	*/
		data = *pdata++;			/* Asigna el caracter apuntado a
									   dato y luego incrementa puntero 	*/
		TxPutChar(data);			/* impone un caracter al buffer		*/
	}								/* Fin de llave de cadena 			*/
}

/*
*******************************************************************************
*
*  						IMPONE UN BYTE EN EL BUFFER DE RECEPCIÓN
*
*  Descripción  	: Mueve 1 byte al buffer de recepción.  Instalar esta
*					  rutina junto a SCIRxGet() en la subrutina de recepción.
*  Argumentos   	: data		es el byte a agregar al buffer de recepción.
*  Retorno			: Ninguno
*  Notas    		: Si el buffer está lleno, el caracter es ignorado
*******************************************************************************
*/

void 	FIFO::RxPutChar(INT16S data)
{									/* Impone 1 byte en el buffer		*/
	if (fifo.RxBufCtr < FIFO_RX_MAX) {
									/* Si existe capacidad en el buffer	*/
		fifo.RxBufCtr++;		/* Incremento contador				*/
		*fifo.RxBufHd++ = data;
									/* Agrego byte al buffer y apunto
									   al siguiente						*/
    if (fifo.RxBufHd == &fifo.RxBuf[FIFO_RX_MAX])
									/* Si la cabeza llega al final		*/
			fifo.RxBufHd = &fifo.RxBuf[0];
									/* Apunto a la primera posición		*/
	}								/* Fin de la condición				*/
}

/*
*******************************************************************************
*
*  						REMUEVE UN BYTE DEL BUFFER DE RECEPCIÓN
*
*  Descripción  	: Adquiere 1 byte del buffer de Recepción, byte recibido.
*  Argumentos   	: Ninguno
*  Retorno			: Caracter extraido del buffer de Rx, a recibir del buffer.
*  Notas    		: Si el buffer está vacío retorna un valor nulo (0)
*******************************************************************************
*/

INT16S	FIFO::RxGetChar(void)
{									/* Recoge 1 byte del buffer			*/
	INT16S temp;						/* Variable temporal				*/


//	DisableInterrupts;				/* sei (CCR_I = 1)					*/
	temp = 0;						/* valor inicial					*/
	if (fifo.RxBufCtr > 0) {	/* Si el buffer no está vacío		*/
		fifo.RxBufCtr--;		/* Decremento contador				*/
		temp = *fifo.RxBufTl++;

		if (fifo.RxBufTl == &fifo.RxBuf[FIFO_RX_MAX])
			fifo.RxBufTl = &fifo.RxBuf[0];
	}
//	EnableInterrupts;				/* cli (CCR_I = 0)					*/
	return (temp);
}

/*
*******************************************************************************
*
*  					ADQUIERE UNA LÍNEA DE CARACTERES POR SERIAL
*
*  Descripción  	: Adquiere una línea de caracteres por serial.
*  Argumentos   	: pdata		es el inicio de depósito de la data recibida
*  Retorno			: Ninguno
*  Notas    		: Para un buen funcionamiento se sugiere que la data envia
*					  da termine en 0, para poder recoger todos los caracteres
*******************************************************************************
*/

void    FIFO::RxGetLine(INT16S *pdata)
{									/* Recibe una trama de bytes		*/
	INT16S data;						/* Data temporal para remover info	*/


	data = 0;
	do {
		data = RxGetChar();
		if (data != 13 && data)		/* Mientras no sea fin de línea	o
									   la tecla Enter					*/
			*pdata++ = data;		/* Mueve a la posición de memoria	*/
	} while (data);					/* Fin llave do while				*/
}

/*
*******************************************************************************
*
*  		ENVÍA UNA LÍNEA DE CARACTERES AL SERIAL POR EL PUERTO SERIAL
*
*  Descripción  	: Envía una cadena por el puerto serial.
*  Argumentos   	: pdata		es la posición inicial de la trama a enviar
*  Retorno			: Ninguno
*  Notas    		: Ninguna
*******************************************************************************
*/

void FIFO::RxPutLine(INT16S *pdata)
{									/* Envía una trama de bytes			*/
	INT16S data;						/* Variable temporal del caracter 	*/


	data = 0;						/* Inicializa data en 0				*/
	while (*pdata != '\0') {		/* Mientras el caracter a enviar
									   sea diferente al final de cadena	*/
		data = *pdata++;			/* Asigna el caracter apuntado a
									   dato y luego incrementa puntero 	*/
		RxPutChar(data);			/* impone un caracter al buffer		*/
	}								/* Fin de llave de cadena 			*/
}

INT16S FIFO::RxGetBytes(void) {
    return fifo.RxBufCtr;
}
