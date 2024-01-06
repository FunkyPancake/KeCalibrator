/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "fsl_common.h"
#include "fsl_flexcan.h"
#include "fsl_clock.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_freertos.h"
#include "fsl_adc12.h"
#include "fsl_pdb.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Used DMA device. */
#define DMA_DMA_BASEADDR DMA0
/* Associated DMAMUX device that is used for muxing of requests. */
#define DMA_DMAMUX_BASEADDR DMAMUX

  /* Channel CH0 definitions */
/* DMA eDMA source request. */
#define DMA_CH0_DMA_REQUEST kDmaRequestMux0FTM0Channel1
/* Selected eDMA channel number. */
#define DMA_CH0_DMA_CHANNEL 0
/* NVIC interrupt vector ID (number). */
#define PDB1_IRQN_IRQN PDB1_IRQn
/* NVIC interrupt vector priority. */
#define PDB1_IRQN_IRQ_PRIORITY 2
/* NVIC interrupt handler identifier. */
#define PDB1_IRQN_IRQHANDLER PDB1_IRQHandler
/* Definition of peripheral ID */
#define CAN0_PERIPHERAL CAN0
/* Definition of the clock source frequency */
#define CAN0_CLOCK_SOURCE 84000000UL
/* BOARD_InitPeripherals defines for LPSPI0 */
/* Definition of peripheral ID */
#define LPSPI0_PERIPHERAL LPSPI0
/* Definition of clock source */
#define LPSPI0_CLOCK_FREQ 21000000UL
/* LPSPI0 interrupt vector ID (number). */
#define LPSPI0_IRQN LPSPI0_IRQn
/* LPSPI0 interrupt vector priority. */
#define LPSPI0_IRQ_PRIORITY 5
/* Alias for ADC0 peripheral */
#define ADC0_PERIPHERAL ADC0
/* ADC0 interrupt vector ID (number). */
#define ADC0_IRQN ADC0_IRQn
/* ADC0 interrupt vector priority. */
#define ADC0_IRQ_PRIORITY 3
/* ADC0 interrupt handler identifier. */
#define ADC0_IRQHANDLER ADC0_IRQHandler
/* BOARD_InitPeripherals defines for LPSPI1 */
/* Definition of peripheral ID */
#define LPSPI1_PERIPHERAL LPSPI1
/* Definition of clock source */
#define LPSPI1_CLOCK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpspi1)
/* LPSPI1 interrupt vector ID (number). */
#define LPSPI1_IRQN LPSPI1_IRQn
/* LPSPI1 interrupt vector priority. */
#define LPSPI1_IRQ_PRIORITY 2
/* BOARD_InitPeripherals defines for PDB1 */
/* Definition of peripheral ID */
#define PDB1_PERIPHERAL PDB1
/* Definition of the ADC pre-trigger (PT) configurations. Used as index to PDB1_ADC0_pretriggers_value arrays. */
enum PDB1_adc0_trigger_config_indexes
{
  PDB1_ADC0_PT0 = 0U
};
/* Number of the pre-triggers used in appropriate configuration` */
#define PDB1_ADC0_PRETRIGGERS_COUNT 1
/* Number of the CMP widow sample pulse out configuration */
#define PDB1_DAC_TRIGGERS_COUNT 0
/* Number of the CMP widow sample pulse out configuration */
#define PDB1_CMP_PULSE_OUT_COUNT 0

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const edma_config_t DMA_config;
extern const flexcan_config_t CAN0_config;
/* Message buffer 0 configuration structure */
extern const flexcan_rx_mb_config_t CAN0_rx_mb_config_0;
extern flexcan_rx_fifo_config_t CAN0_rx_fifo_config;
extern const lpspi_master_config_t LPSPI0_config;
extern lpspi_rtos_handle_t LPSPI0_handle;
extern const adc12_config_t ADC0_config;
extern adc12_channel_config_t ADC0_channelsConfig[1];
extern const adc12_hardware_average_mode_t ADC0_hardwareAverageConfig;
extern const lpspi_master_config_t LPSPI1_config;
extern lpspi_rtos_handle_t LPSPI1_handle;
extern const pdb_config_t PDB1_config;
extern pdb_adc_pretrigger_config_t PDB1_ADC0_pretriggers_config;
extern const uint32_t PDB1_ADC0_pretriggers_value[PDB1_ADC0_PRETRIGGERS_COUNT];

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */