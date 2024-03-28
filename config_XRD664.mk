TOC									= 0x11184c8 # static throughout
LOADER_INJECT_ADDR 					= 0x10250  # nop in main()
LOADER_START_ADDR 					= 0xa7bde0 # align16 @ criAtomTblAcb_GetItem
LOADER_END_ADDR 					= 0xA7BE60 # LOADER_START_ADDR + 0x80
LOADER_SYS_PRX_MODULE_LOAD_ADDR 	= 0xdf4bc8 # cellPrxForUser _sys_heap_delete_heap import
LOADER_SYS_PRX_MODULE_START_ADDR 	= 0xdf4aa8 # cellPrxForUser _sys_heap_malloc import

# 745 instructions, around ~200 hooks
HOOK_SHARED_TEXT_BEGIN_ADDR = $(LOADER_END_ADDR)
HOOK_SHARED_TEXT_END_ADDR 	= 0xA7CA04 # HOOK_SHARED_TEXT_BEGIN_ADDR + 0xBA4

# 0xA8 / 4 = 42 words
HOOK_SHARED_DATA_BEGIN_ADDR = 0x108a504
HOOK_SHARED_DATA_END_ADDR 	= 0x108A5AC # HOOK_SHARED_DATA_BEGIN_ADDR + 0xA8