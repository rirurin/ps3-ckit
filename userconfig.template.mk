PYTHON = py -3
RPCS3_DIR = X:\emu\rpcs3
GAME = P5
GAME_ID = NPEB02436
GAME_DIR = $(RPCS3_DIR)\dev_hdd0\game\$(GAME_ID)\USRDIR
PATCH_FILE = $(RPCS3_DIR)\patches\patch.yml

# hooks
# todo move this shit elsewhere
HOOK_SET_BGM = setBgm/0x6CF04/"stdu  r1, -0x80(r1)"
HOOK_SET_SEQ = setSeq/0x10DB4/"stdu  r1, -0xB0(r1)"
HOOKS = $(HOOK_SET_BGM) $(HOOK_SET_SEQ)