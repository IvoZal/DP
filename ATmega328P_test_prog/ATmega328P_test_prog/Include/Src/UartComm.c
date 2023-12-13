/*
 * UartComm.c
 *
 * Created: 09.12.2023 21:44:08
 * Author: Ivo Zaludek
 */ 

#include "UartComm.h"

static bool bUserFlag = false;
static char sUserName[MAX_USERNAME_LEN] = {'\0'};
char cOpt;

void UartHandler(uint8_t u8Status, uint8_t * u8Unlock, uint8_t * u8Lock)
{
	uint8_t u8Time[3];
	RTC_read_time(u8Time);
	switch (u8Status)
	{
		case 1:
			printf("AutoOpen,%u:%u:%u\n",u8Time[0],u8Time[1],u8Time[2]);
			break;
			
		case 2:
			printf("UserOpen,%u:%u:%u\n",u8Time[0],u8Time[1],u8Time[2]);
			break;
			
		case 3:
			printf("Lock,%u:%u:%u\n",u8Time[0],u8Time[1],u8Time[2]);
			break;
			
		case 4:
			printf("AlarmOn,%u:%u:%u\n",u8Time[0],u8Time[1],u8Time[2]);
			break;
			
		case 5:
			printf("AlarmOff,%u:%u:%u\n",u8Time[0],u8Time[1],u8Time[2]);
			break;
			
		case 6:
			printf("PowerOn,%u:%u:%u\n",u8Time[0],u8Time[1],u8Time[2]);
			break;
	}
	if(BufferLength() > 0)
	{
		switch (BufferGet())
		{
			case 'T':
				cOpt = BufferGet();
				uint8_t u8SetTime[3];
				for(uint8_t u8i = 0; u8i < 3; u8i++) {
					BufferGet();
					u8SetTime[u8i] = (BufferGet() - 0x30) * 10;
					u8SetTime[u8i] += BufferGet() - 0x30;
				}
				switch (cOpt)
				{
					case 'S':
						RTC_set_time(u8SetTime);
						printf("Time was set.\n");
						break;
						
					case 'U':
						memcpy(u8Unlock, u8SetTime, 3);
						SaveTimes(u8Unlock,u8Lock);
						printf("Auto unlock time set.\n");
						break;
						
					case 'L':
						memcpy(u8Lock, u8SetTime, 3);
						SaveTimes(u8Unlock,u8Lock);
						printf("Auto lock time set.\n");
						break;
				}
				break;
				
			case 'U':
				bUserFlag = true;
				BufferGet();
				for(uint8_t u8j = 0; (BufferLength() > 0) & (u8j < MAX_USERNAME_LEN); u8j++) {
					sUserName[u8j] = BufferGet();
				}
				break;
				
			case 'P':
				if(bUserFlag == true) {
					char sPassw[MAX_PASSW_LEN] = {'\0'};
					BufferGet();
					uint8_t u8j = 0;
					for(; (BufferLength() > 0) & (u8j < MAX_PASSW_LEN); u8j++) {
						sPassw[u8j] = BufferGet();
					}
					if(u8j > 3) {		// password is longer than 4 chars
						bUserFlag = false;
						if(SaveUser(sUserName,sPassw))
							printf("User %s added.\n",sUserName);
						else
							printf("Memory is full.\n");
						for (uint8_t u8i = 0; u8i < MAX_USERNAME_LEN; u8i++)
							sUserName[u8i] = '\0';
					}
					else
						printf("Password must be longer than 4 chars.\n");					
					break;
				}
				else {
					while(BufferLength() > 0)
						BufferGet();
				}
				
			case 'D':
				BufferGet();
				char sDelUserName[MAX_USERNAME_LEN] = {'\0'};
				for(uint8_t u8j = 0; (BufferLength() > 0) & (u8j < MAX_USERNAME_LEN); u8j++) {
					sDelUserName[u8j] = BufferGet();
				}
				bool bDelFlag = DeleteUser(sDelUserName);
				if(bDelFlag)
					printf("User %s deleted.\n",sDelUserName);
				else
					printf("Username not found.\n");
				break;
				
			case 'E':
				if(BufferGet() == 'M') {
					DeleteAll();
					printf("Memory erased.\n");
				}
				
			default:
				while(BufferLength() > 0)
					BufferGet();
		}
	}
}