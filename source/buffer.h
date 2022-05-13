/*
 * ringBuffer.h
 *
 *  Created on: 17/03/2022
 *      Author: Nicolas
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "stdint.h"
#include "stdlib.h"
#include "varios.h"
#include "MKL46Z4.h"

typedef struct {
	int32_t count, size, indexRead, indexWrite;
	uint8_t *pBuf;
} buffer_struct;

/** @brief inicializa buffer
 **
 ** @param size tamaño del buffer
 **
 ** @return puntero a buffer , NULL si no se pudo crear
 **/
buffer_struct* buffer_init(int32_t size);

/** @brief pone nuevo dato en el buffer
 ** si el buffer esta lleno se pisa el dato más antiguo
 ** y se avanza el puntero de lectura
 **
 ** @param[out] pRb puntero al buffer
 ** @param[in] data dato a colocar en el buffer
 **
 ** @return true si se pudo poner el dato correctamente
 **         false si se puso y se piso un dato antiguo
 **/
char buffer_add(buffer_struct *pRb, uint8_t data);

/** @brief devuelve dato más antiguo del buffer
 **
 ** @param[out] pRb puntero al buffer
 ** @param[in] data puntero a donde guardar el dato
 **
 ** @return true si se extrajo el dato del buffer
 **         false si no había datos en el buffer
 **/
char buffer_get_data(buffer_struct *pRb, uint8_t *dp);

/** \brief Peeks a ring buffer, i.e. returns an element without removing it.
 *
 * \param cp The buffer from which the data should be returned.
 * \param dp A pointer to the location at which the data should be placed.
 * \param index The index to peek.
 * \return 1 if data was returned; 0 otherwise.
 *
 */
char buffer_peek(buffer_struct *pRb, uint8_t *dp, int32_t index);

int32_t buffer_get_count(buffer_struct *pRb);

void buffer_reset(buffer_struct *pRb);

/** @brief libera de memoria buffer
 **
 ** @param pRb puntero al buffer
 **
 **/
void buffer_deInit(buffer_struct *pRb);

#define buffer_available(BUFFER) ((BUFFER)->size - buffer_get_count((BUFFER)))
#define buffer_is_empty(BUFFER) ((BUFFER)->count == 0) //pRb->count == 0
#define buffer_is_full(BUFFER) ((BUFFER)->count == (BUFFER)->size)

#endif /* BUFFER_H_ */
