/**
 ******************************************************************************
 * @file    USB_Device/MSC_Standalone/Src/usbd_conf.c
 * @author  MCD Application Team
 * @brief   This file implements the USB Device library callbacks and MSP
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------ */
#include "stm32mp1xx.h"
#include "usbd_core.h"
#include "usbd_msc.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
PCD_HandleTypeDef hpcd;

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/*******************************************************************************
					   PCD BSP Routines
*******************************************************************************/

/**
 * @brief  Initializes the PCD MSP.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd) {
	GPIO_InitTypeDef GPIO_InitStruct;

	if (hpcd->Instance == USB_OTG_HS) {
		// __HAL_RCC_GPIOA_CLK_ENABLE();

		// /* Configure DM DP Pins */
		// GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
		// GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		// GPIO_InitStruct.Pull = GPIO_NOPULL;
		// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		// GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
		// HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// /* Configure VBUS Pin */
		// GPIO_InitStruct.Pin = GPIO_PIN_9;
		// GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		// GPIO_InitStruct.Pull = GPIO_NOPULL;
		// HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// /* Configure ID pin */
		// GPIO_InitStruct.Pin = GPIO_PIN_10;
		// GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		// GPIO_InitStruct.Pull = GPIO_PULLUP;
		// GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
		// HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* Enable USB HS Clocks */
		//__HAL_RCC_USBO_CONFIG(RCC_USBOCLKSOURCE_PHY);

		__HAL_RCC_USBO_FORCE_RESET();
		__HAL_RCC_USBO_RELEASE_RESET();
		__HAL_RCC_USBPHY_FORCE_RESET();
		__HAL_RCC_USBPHY_RELEASE_RESET();

		__HAL_RCC_USBO_CLK_ENABLE();
		// __HAL_RCC_USBH_CLK_ENABLE();
		__HAL_RCC_USBPHY_CLK_ENABLE();

		// __HAL_RCC_USBPHY_CLK_SLEEP_DISABLE();
		// __HAL_RCC_USBH_CLK_SLEEP_DISABLE();
		// __HAL_RCC_USBO_CLK_SLEEP_DISABLE();

		GIC_SetTarget(OTG_IRQn, 1);
		GIC_SetPriority(OTG_IRQn, 0b01111000);
		GIC_SetConfiguration(OTG_IRQn, 0b10); // Edge triggered
		GIC_EnableIRQ(OTG_IRQn);

		// HAL_NVIC_SetPriority(OTG_IRQn, 6, 0);
		// HAL_NVIC_EnableIRQ(OTG_IRQn);
	}
}

/**
 * @brief  De-Initializes the PCD MSP.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd) {
	if (hpcd->Instance == USB_OTG_HS) {
		__HAL_RCC_USBO_CLK_DISABLE();
	}
}

/*******************************************************************************
					   LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

/**
 * @brief  SetupStage callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {
	USBD_LL_SetupStage(hpcd->pData, (uint8_t *)hpcd->Setup);
}

/**
 * @brief  DataOut Stage callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
	USBD_LL_DataOutStage(hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
 * @brief  DataIn Stage callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
	USBD_LL_DataInStage(hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
 * @brief  SOF callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd) {
	USBD_LL_SOF(hpcd->pData);
}

/**
 * @brief  Reset callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {
	USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

	/* Set USB Current Speed */
	switch (hpcd->Init.speed) {
		case PCD_SPEED_HIGH:
			speed = USBD_SPEED_HIGH;
			break;

		case PCD_SPEED_FULL:
			speed = USBD_SPEED_FULL;
			break;

		default:
			speed = USBD_SPEED_FULL;
			break;
	}

	/* Reset Device */
	USBD_LL_Reset(hpcd->pData);

	USBD_LL_SetSpeed(hpcd->pData, speed);
}

/**
 * @brief  Suspend callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {
	USBD_LL_Suspend(hpcd->pData);
}

/**
 * @brief  Resume callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) {
	USBD_LL_Resume(hpcd->pData);
}

/**
 * @brief  ISOOUTIncomplete callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
	USBD_LL_IsoOUTIncomplete(hpcd->pData, epnum);
}

/**
 * @brief  ISOINIncomplete callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
	USBD_LL_IsoINIncomplete(hpcd->pData, epnum);
}

/**
 * @brief  ConnectCallback callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) {
	USBD_LL_DevConnected(hpcd->pData);
}

/**
 * @brief  Disconnect callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) {
	USBD_LL_DevDisconnected(hpcd->pData);
}

/*******************************************************************************
					   LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

/**
 * @brief  Initializes the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev) {
	/* Set LL Driver parameters */
	hpcd.Instance = USB_OTG_HS;
	hpcd.Init.dev_endpoints = 9;
	hpcd.Init.use_dedicated_ep1 = 0;

	/* Be aware that enabling DMA mode will result in data being sent only by
	 * multiple of 4 packet sizes. This is due to the fact that USB DMA does not
	 * allow sending data from non word-aligned addresses. For this specific
	 * application, it is advised to not enable this option unless required. */
	hpcd.Init.dma_enable = 0;
	hpcd.Init.low_power_enable = 0;
	hpcd.Init.lpm_enable = 0;
	hpcd.Init.phy_itface = USB_OTG_HS_EMBEDDED_PHY;//PCD_PHY_ULPI;
	hpcd.Init.Sof_enable = 0;
	hpcd.Init.speed = PCD_SPEED_HIGH;
	hpcd.Init.vbus_sensing_enable = 0;

	/* Link The driver to the stack */
	hpcd.pData = pdev;
	pdev->pData = &hpcd;

	// Force to null, so ST USB library doesn't try to de-init it
	((USBD_HandleTypeDef *)hpcd.pData)->pClassData = NULL;

	/* Initialize LL Driver */
	if (HAL_PCD_Init(&hpcd) != HAL_OK)
		__BKPT(9);

	HAL_PCDEx_SetRxFiFo(&hpcd, 0x200);
	HAL_PCDEx_SetTxFiFo(&hpcd, 0, 0x40);
	HAL_PCDEx_SetTxFiFo(&hpcd, 1, 0x100);

	return USBD_OK;
}

/**
 * @brief  De-Initializes the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev) {
	HAL_PCD_DeInit(pdev->pData);
	return USBD_OK;
}

/**
 * @brief  Starts the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev) {
	HAL_PCD_Start(pdev->pData);
	return USBD_OK;
}

/**
 * @brief  Stops the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev) {
	HAL_PCD_Stop(pdev->pData);
	return USBD_OK;
}

/**
 * @brief  Opens an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @param  ep_type: Endpoint Type
 * @param  ep_mps: Endpoint Max Packet Size
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps) {
	HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);

	return USBD_OK;
}

/**
 * @brief  Closes an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
	HAL_PCD_EP_Close(pdev->pData, ep_addr);
	return USBD_OK;
}

/**
 * @brief  Flushes an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
	HAL_PCD_EP_Flush(pdev->pData, ep_addr);
	return USBD_OK;
}

/**
 * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
	HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
	return USBD_OK;
}

/**
 * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
	HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);
	return USBD_OK;
}

/**
 * @brief  Returns Stall condition.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval Stall (1: Yes, 0: No)
 */
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
	PCD_HandleTypeDef *hpcd = pdev->pData;

	if ((ep_addr & 0x80) == 0x80) {
		return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	} else {
		return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	}
}

/**
 * @brief  Assigns a USB address to the device.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr) {
	HAL_PCD_SetAddress(pdev->pData, dev_addr);
	return USBD_OK;
}

/**
 * @brief  Transmits data over an endpoint.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @param  pbuf: Pointer to data to be sent
 * @param  size: Data size
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t size) {
	HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}

/**
 * @brief  Prepares an endpoint for reception.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @param  pbuf: Pointer to data to be received
 * @param  size: Data size
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t size) {
	HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}

/**
 * @brief  Returns the last transferred packet size.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval Received Data Size
 */
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr) {
	return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}

/**
 * @brief  Static single allocation.
 * @param  size: Size of allocated memory
 * @retval None
 */
void *USBD_static_malloc(uint32_t size) {
	static uint32_t mem[(sizeof(USBD_MSC_BOT_HandleTypeDef) / 4) + 1]; /* On 32-bit boundary */
	return mem;
}

/**
 * @brief  Dummy memory free
 * @param  p: Pointer to allocated  memory address
 * @retval None
 */
void USBD_static_free(void *p) {
}

/**
 * @brief  Delays routine for the USB Device Library.
 * @param  Delay: Delay in ms
 * @retval None
 */
void USBD_LL_Delay(uint32_t Delay) {
	HAL_Delay(Delay);
}
