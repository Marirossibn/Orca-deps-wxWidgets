#include <Types.r>

data 'alis' (128, "Libs alias") {
	$"0000 0000 00CE 0002 0001 0A58 5858 5858"           
	$"5858 5858 5800 0000 0000 0000 0000 0000"            
	$"0000 0000 0000 AB9F 536B 4244 0000 0002"            
	$"EE7C 044C 6962 7300 0000 0000 0000 0000"            
	$"0000 0000 0000 0000 0000 0000 0000 0000"            
	$"0000 0000 0000 0000 0000 0000 0000 0000"          
	$"0000 0000 0000 0000 0000 0000 0000 0000"            
	$"0000 0003 1F71 AE82 7707 006A 0016 026C"         
	$"01A3 0001 0001 0000 0000 0000 0000 0000"            
	$"0000 0000 0000 0000 0009 5858 5858 5858"            
	$"5858 5800 0001 0004 0002 EE7C 0002 0019"            
	$"5858 5858 5858 5858 5858 5858 5858 5858"           
	$"5858 5858 5858 5858 5800 FFFF 0000"                 
};

data 'CSOR' (0) {
	$"1441 6476 616E 6365 6420 436F 6E63 6570"            
	$"7473 2041 47"                                       
};

resource 'FREF' (128) {
	'APPL',
	0,
	""
};

resource 'FREF' (129) {
	'shlb',
	1,
	""
};

resource 'FREF' (130) {
	'PLPW',
	2,
	""
};

resource 'icl4' (128) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0666 6666 0000 0000 0000 0006 6666 6660"
	$"0666 6666 0000 0000 0000 0006 6666 6660"
	$"0666 6660 0000 0000 0000 0000 6666 6660"
	$"0666 6660 0000 0006 6000 0000 6666 6660"
	$"0666 6660 0000 0666 6600 0000 6666 6660"
	$"0666 6660 0000 6606 6600 0000 6666 6660"
	$"0666 6660 0000 6606 0000 0000 6666 6660"
	$"0666 6600 0000 0666 6000 0000 0666 6660"
	$"0666 6600 0000 0006 6600 0000 0666 6660"
	$"0666 6600 0000 0006 0060 0000 0666 6660"
	$"0666 6660 0000 0006 6600 0000 6666 6660"
	$"0666 6660 0000 0666 6000 0000 6666 6660"
	$"0666 6666 0000 6606 0000 0006 6666 6660"
	$"0666 6666 6000 6606 0000 0066 6666 6660"
	$"0666 6666 6000 0666 6600 0066 6666 6660"
	$"0666 6666 6600 0006 0600 0666 6666 6660"
	$"0666 6666 6600 0006 0000 0666 6666 6660"
	$"0666 6666 6660 0006 0000 6666 6666 6660"
	$"0666 6666 6666 0006 0006 6666 6666 6660"
	$"0666 6666 6666 0006 0006 6666 6666 6660"
	$"0666 6666 6666 6006 0066 6666 6666 6660"
	$"0666 6666 6666 6006 0066 6666 6666 6660"
	$"0666 6666 6666 6006 0066 6666 6666 6660"
	$"0666 6666 6666 6606 0666 6666 6666 6660"
	$"0666 6666 6666 6606 0666 6666 6666 6660"
	$"0666 6666 6666 6606 0666 6666 6666 6660"
	$"0666 6666 6666 6606 0666 6666 6666 6660"
	$"0666 6666 6666 6660 6666 6666 6666 6660"
	$"0666 6666 6666 6666 6666 6666 6666 6660"
	$"0666 6666 6666 6666 6666 6666 6666 6660"
	$"0666 6666 6666 6666 6666 6666 6666 6660"
};

resource 'icl4' (129, "Editor icon-generic  ") {
	$"0FFF FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"F000 0000 0000 0000 0000 0000 000C F000"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"F0CC CCCF FFFF FFFF FFFF FFFF CCCC F000"
	$"F0CC CCFD DDDD DDDD DDDD DDDD FCCC F000"
	$"F0CC CCFD DDDD DDDF FDDD DDDD FCCC F000"
	$"F0CC CFDD DDDD DFFF FFDD DDDD DFCC F000"
	$"F0FC CFDD DDDD FFCF FFDD DDDD DFCC F000"
	$"FF0F 0FDD DDDD FFCF DDDD DDDD DFCC F0F0"
	$"F00F 0FFD DDDD DFFF FDDD DDDD FFCC FFCF"
	$"000F 0CFD DDDD DDDF FFDD DDDD FCCC F0CF"
	$"000F 0CCF DDDD DDDF DDDD DDDF CCCC CCCF"
	$"000F 0CCF FDDD DDDF FFDD DDFC CCCC CCCF"
	$"000F 0CCC FDDD DFFF FDDD DDFC CCCC CCCF"
	$"000F 0CCC CFDD FFCF DDDD DFCC CCCC CCCF"
	$"000F 0CCC CFDD FFCF DDDD DFCC CCCC CCCF"
	$"000F 0CCC CCFD DFFF FFDD FCCC CCCC CCCF"
	$"000F 0CCC CCFF DDDF DFDF CCCC CCCC CCCF"
	$"000F 0CCC CCCF DDDF DDDF CCCC CCCC CCCF"
	$"000F 0CCC CCCC FDDF DDFC CCCC CCCC FCCF"
	$"F00F 0CCC CCCC FDDF DDFC CCCC CCCC FFCF"
	$"FF0F 0CCC CCCC FDDF DDFC CCCC CCCC F0F0"
	$"F0F0 CCCC CCCC CFDF DFCC CCCC CCCC F000"
	$"F00C CCCC CCCC CFDF DFCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CFDF DFCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CCFF FCCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"FCCC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"0FFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

resource 'icl4' (130, "Editor icon-generic  ") {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"000F FFFF FFFF FFFF FFFF FFFF FFFF F000"
	$"000F 0000 0000 0000 0000 0000 0000 F000"
	$"000F 0CCC CCCC CCCC CCCC CCCC CCCC F000"
	$"FFFF DDDD DDDD DDDD DDDD DDDD DDDD FFFF"
	$"F00D 0000 0000 0000 0000 0000 0000 D00F"
	$"F0CD 0CFD DDDD DDDD DDDD DDDD FCCC DCCF"
	$"F0CD 0CFD DDDD DDDF FDDD DDDD FCCC DCCF"
	$"F0CD 0FDD DDDD DFFF FFDD DDDD DFCC DCCF"
	$"F0CD 0FDD DDDD FFCF FFDD DDDD DFCC DCCF"
	$"F0CD 0FDD DDDD FFCF DDDD DDDD DFCC DCCF"
	$"F0CD 0FFD DDDD DFFF FDDD DDDD FFCC DCCF"
	$"F0CD 0CFD DDDD DDDF FFDD DDDD FCCC DCCF"
	$"F0CD 0CCF DDDD DDDF DDDD DDDF CCCC DCCF"
	$"F0CD 0CCF FDDD DDDF FFDD DDFC CCCC DCCF"
	$"F0CD 0CCC FDDD DFFF FDDD DDFC CCCC DCCF"
	$"F0CD 0CCC CFDD FFCF DDDD DFCC CCCC DCCF"
	$"F0CD 0CCC CFDD FFCF DDDD DFCC CCCC DCCF"
	$"F0CD 0CCC CCFD DFFF FFDD FCCC CCCC DCCF"
	$"F0CD 0CCC CCFF DDDF DFDF CCCC CCCC DCCF"
	$"F0CD 0CCC CCCF DDDF DDDF CCCC CCCC DCCF"
	$"F0CD 0CCC CCCC FDDF DDFC CCCC CCCC DCCF"
	$"F0CD 0CCC CCCC FDDF DDFC CCCC CCCC DCCF"
	$"F0CD 0CCC CCCC FDDF DDFC CCCC CCCC DCCF"
	$"F0CD 0CCC CCCC CFDF DFCC CCCC CCCC DCCF"
	$"F0CD 0CCC CCCC CFDF DFCC CCCC CCCC DCCF"
	$"F0CD 0CCC CCCC CFDF DFCC CCCC CCCC DCCF"
	$"F0CD DDDD DDDD DDFF FDDD DDDD DDDD FFFF"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"F0CC CCCC CCCC CCCC CCCC CCCC CCCC F000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF F0"
};

resource 'icl8' (128) {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"00AB ABAB ABAB ABAB 0000 0000 0000 0000"
	$"0000 0000 0000 00AB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB 0000 0000 0000 0000"
	$"0000 0000 0000 00AB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 0000 0000"
	$"0000 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 0000 00AB"
	$"AB00 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 00AB ABAB"
	$"ABAB 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 ABAB 00AB"
	$"ABAB 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 ABAB 00AB"
	$"0000 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB 0000 0000 0000 00AB ABAB"
	$"AB00 0000 0000 0000 00AB ABAB ABAB AB00"
	$"00AB ABAB ABAB 0000 0000 0000 0000 00AB"
	$"ABAB 0000 0000 0000 00AB ABAB ABAB AB00"
	$"00AB ABAB ABAB 0000 0000 0000 0000 00AB"
	$"0000 AB00 0000 0000 00AB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 0000 00AB"
	$"ABAB 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB AB00 0000 0000 00AB ABAB"
	$"AB00 0000 0000 0000 ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB 0000 0000 ABAB 00AB"
	$"0000 0000 0000 00AB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB AB00 0000 ABAB 00AB"
	$"0000 0000 0000 ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB AB00 0000 00AB ABAB"
	$"ABAB 0000 0000 ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB 0000 0000 00AB"
	$"00AB 0000 00AB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB 0000 0000 00AB"
	$"0000 0000 00AB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB AB00 0000 00AB"
	$"0000 0000 ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB 0000 00AB"
	$"0000 00AB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB 0000 00AB"
	$"0000 00AB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB AB00 00AB"
	$"0000 ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB AB00 00AB"
	$"0000 ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB AB00 00AB"
	$"0000 ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB 00AB"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB 00AB"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB 00AB"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB 00AB"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"ABAB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB ABAB"
	$"ABAB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB ABAB"
	$"ABAB ABAB ABAB ABAB ABAB ABAB ABAB AB00"
	$"00AB ABAB ABAB ABAB ABAB ABAB ABAB ABAB"
	$"ABAB ABAB ABAB ABAB ABAB ABAB ABAB AB"
};

resource 'icl8' (130, "Editor icon-generic  ") {
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 0000 0000"
	$"0000 00FF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FF00 0000"
	$"0000 00FF 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 00F5 FF00 0000"
	$"0000 00FF F5F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F8 FF00 0000"
	$"FFFF FFFF F9F9 F9F9 F9F9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 F9F9 F9F9 F9F9 F9F9 FFFF FFFF"
	$"FF00 F5F9 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 0000 F9F5 00FF"
	$"FF00 F6F9 002B FFF9 F9F9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 F9F9 F9F9 FF2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B FFF9 F9F9 F9F9 F9F9 F9FF"
	$"FFF9 F9F9 F9F9 F9F9 FF2B 2BF8 F92B F8FF"
	$"FF00 F6F9 00FF F9F9 F9F9 F9F9 F9FF FFFF"
	$"FFFF F9F9 F9F9 F9F9 F9FF 2BF8 F92B F8FF"
	$"FF00 F6F9 00FF F9F9 F9F9 F9F9 FFFF 2BFF"
	$"FFFF F9F9 F9F9 F9F9 F9FF 2BF8 F92B F8FF"
	$"FF00 F6F9 00FF F9F9 F9F9 F9F9 FFFF 2BFF"
	$"F9F9 F9F9 F9F9 F9F9 F9FF 2BF8 F92B F8FF"
	$"FF00 F6F9 00FF FFF9 F9F9 F9F9 F9FF FFFF"
	$"FFF9 F9F9 F9F9 F9F9 FFFF 2BF8 F92B F8FF"
	$"FF00 F6F9 002B FFF9 F9F9 F9F9 F9F9 F9FF"
	$"FFFF F9F9 F9F9 F9F9 FF2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2BFF F9F9 F9F9 F9F9 F9FF"
	$"F9F9 F9F9 F9F9 F9FF 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2BFF FFF9 F9F9 F9F9 F9FF"
	$"FFFF F9F9 F9F9 FF2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B FFF9 F9F9 F9FF FFFF"
	$"FFF9 F9F9 F9F9 FF2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2BFF F9F9 FFFF 2BFF"
	$"F9F9 F9F9 F9FF 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2BFF F9F9 FFFF 2BFF"
	$"F9F9 F9F9 F9FF 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B FFF9 F9FF FFFF"
	$"FFFF F9F9 FF2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B FFFF F9F9 F9FF"
	$"F9FF F9FF 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B 2BFF F9F9 F9FF"
	$"F9F9 F9FF 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B 2B2B FFF9 F9FF"
	$"F9F9 FF2B 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B 2B2B FFF9 F9FF"
	$"F9F9 FF2B 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B 2B2B FFF9 F9FF"
	$"F9F9 FF2B 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B 2B2B 2BFF F9FF"
	$"F9FF 2B2B 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 002B 2B2B 2B2B 2B2B 2BFF F9FF"
	$"F9FF 2B2B 2B2B 2B2B 2B2B 2BF8 F92B F8FF"
	$"FF00 F6F9 00F8 F82B 2B2B 2B2B 2BFF F9FF"
	$"F9FF 2B2B 2B2B 2B2B F8F8 F8F8 F9F7 F8FF"
	$"FF00 F6F9 F9F9 F9F9 F9F9 F9F9 F9F9 FFFF"
	$"FFF9 F9F9 F9F9 F9F9 F9F9 F9F9 FFFF FFFF"
	$"FF00 F62B 2B2B 2B2B 2B2B 2B2B 2B2B 2B2B"
	$"2B2B 2B2B 2B2B 2B2B 2B2B 2BF8 FF00 0000"
	$"FFF5 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 F8F8"
	$"F8F8 F8F8 F8F8 F8F8 F8F8 F8F8 FF00 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FF"
};

resource 'icl8' (129) {
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF 0000 0000"
	$"FF00 0000 0000 0000 0000 0000 0000 0000"
	$"0000 0000 0000 0000 0000 00F6 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6FF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F62B FFF9 F9F9 F9F9 F9F9 F9F9"
	$"F9F9 F9F9 F9F9 F9F9 FF2B F6F7 FF00 0000"
	$"FF00 F6F6 F62B FFF9 F9F9 F9F9 F9F9 F9FF"
	$"FFF9 F9F9 F9F9 F9F9 FF2B F6F7 FF00 0000"
	$"FF00 F7F6 F6FF F9F9 F9F9 F9F9 F9FF FFFF"
	$"FFFF F9F9 F9F9 F9F9 F9FF F6F7 FF00 0000"
	$"FF00 FFF7 F6FF F9F9 F9F9 F9F9 FFFF 2BFF"
	$"FFFF F9F9 F9F9 F9F9 F9FF F6F7 FF00 0000"
	$"FFFF 00FF 00FF F9F9 F9F9 F9F9 FFFF 2BFF"
	$"F9F9 F9F9 F9F9 F9F9 F9FF F6F7 FF00 FF00"
	$"FF00 00FF 00FF FFF9 F9F9 F9F9 F9FF FFFF"
	$"FFF9 F9F9 F9F9 F9F9 FFFF F6F7 FFFF F7FF"
	$"0000 00FF 00F6 FFF9 F9F9 F9F9 F9F9 F9FF"
	$"FFFF F9F9 F9F9 F9F9 FFF6 F6F7 FF00 F7FF"
	$"0000 00FF 00F6 F6FF F9F9 F9F9 F9F9 F9FF"
	$"F9F9 F9F9 F9F9 F9FF F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6FF FFF9 F9F9 F9F9 F9FF"
	$"FFFF F9F9 F9F9 FFF6 F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6F6 FFF9 F9F9 F9FF FFFF"
	$"FFF9 F9F9 F9F9 FFF6 F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6F6 F6FF F9F9 FFFF 2BFF"
	$"F9F9 F9F9 F9FF F6F6 F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6F6 F6FF F9F9 FFFF 2BFF"
	$"F9F9 F9F9 F9FF F6F6 F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6F6 F6F6 FFF9 F9FF FFFF"
	$"FFFF F9F9 FFF6 F6F6 F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6F6 F6F6 FFFF F9F9 F9FF"
	$"F9FF F9FF F6F6 F6F6 F6F6 F6F6 F6F6 F7FF"
	$"0000 00FF 00F6 F6F6 F6F6 F6FF F9F9 F9FF"
	$"F9F9 F9FF F6F6 F6F6 F6F6 F6F6 F7F6 F7FF"
	$"0000 00FF 00F6 F6F6 F6F6 F6F6 FFF9 F9FF"
	$"F9F9 FFF6 F6F6 F6F6 F6F6 F6F7 FFF7 F7FF"
	$"FF00 00FF 00F6 F6F6 F6F6 F6F6 FFF9 F9FF"
	$"F9F9 FFF6 F6F6 F6F6 F6F6 F6F7 FFFF F7FF"
	$"FFFF 00FF 00F6 F6F6 F6F6 F6F6 FFF9 F9FF"
	$"F9F9 FFF6 F6F6 F6F6 F6F6 F6F7 FF00 FF00"
	$"FF00 FF00 F6F6 F6F6 F6F6 F6F6 F6FF F9FF"
	$"F9FF F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 00F6 F6F6 F6F6 F6F6 F6F6 F6FF F9FF"
	$"F9FF F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6FF F9FF"
	$"F9FF F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 FFFF"
	$"FFF6 F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FF00 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6 F6F6"
	$"F6F6 F6F6 F6F6 F6F6 F6F6 F6F7 FF00 0000"
	$"FFF6 F7F7 F7F7 F7F7 F7F7 F7F7 F7F7 F7F7"
	$"F7F7 F7F7 F7F7 F7F7 F7F7 F7F7 FF00 0000"
	$"00FF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
	$"FFFF FFFF FFFF FFFF FFFF FFFF"
};

resource 'icm#' (129) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm#' (130) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'icm4' (129) {
	$""
};

resource 'icm4' (128) {
	$""
};

resource 'icm4' (130) {
	$""
};

resource 'icm8' (129) {
	$""
};

resource 'icm8' (128) {
	$""
};

resource 'icm8' (130) {
	$""
};

resource 'ICN#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 7F00 01FE 7F00 01FE 7E00 00FE"
		$"7E01 80FE 7E07 C0FE 7E0D C0FE 7E0D 00FE"
		$"7C07 807E 7C01 C07E 7C01 207E 7E01 C0FE"
		$"7E07 80FE 7F0D 01FE 7F8D 03FE 7F87 C3FE"
		$"7FC1 47FE 7FC1 07FE 7FE1 0FFE 7FF1 1FFE"
		$"7FF1 1FFE 7FF9 3FFE 7FF9 3FFE 7FF9 3FFE"
		$"7FFD 7FFE 7FFD 7FFE 7FFD 7FFE 7FFD 7FFE"
		$"7FFE FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE",
		/* [2] */
		$"0000 0000 7F00 01FE 7F00 01FE 7E00 00FE"
		$"7E01 80FE 7E07 C0FE 7E0D C0FE 7E0D 00FE"
		$"7C07 807E 7C01 C07E 7C01 207E 7E01 C0FE"
		$"7E07 80FE 7F0D 01FE 7F8D 03FE 7F87 C3FE"
		$"7FC1 47FE 7FC1 07FE 7FE1 0FFE 7FF1 1FFE"
		$"7FF1 1FFE 7FF9 3FFE 7FF9 3FFE 7FF9 3FFE"
		$"7FFD 7FFE 7FFD 7FFE 7FFD 7FFE 7FFD 7FFE"
		$"7FFE FFFE 7FFF FFFE 7FFF FFFE 7FFF FFFE"
	}
};

resource 'ICN#' (129, "Editor icon-generic  ") {
	{	/* array: 2 elements */
		/* [1] */
		$"7FFF FFF0 8000 0008 8000 0008 8000 0008"
		$"82AA AA88 8200 0088 8200 0088 8201 8088"
		$"A407 C048 D40D C04A 940D 004D 1607 80C9"
		$"1201 C081 1101 0101 1181 C201 1087 8201"
		$"104D 0401 104D 0401 1027 C801 1031 5001"
		$"1011 1009 9009 200D D009 200A A009 2008"
		$"8005 4008 8005 4008 8005 4008 8003 8008"
		$"8000 0008 8000 0008 8000 0008 7FFF FFF0",
		/* [2] */
		$"0000 0000 1FFF FFF8 1FFF FFF8 1FFF FFF8"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFF8 FFFF FFF8 FFFF FFF8"
	}
};

resource 'ICN#' (130, "Editor icon-generic  ") {
	{	/* array: 2 elements */
		/* [1] */
		$"0000 0000 1FFF FFF8 1000 0008 1000 0008"
		$"FAAA AAAF 9200 00A1 8200 0089 9201 8081"
		$"8407 C049 940D C041 840D 0049 9607 80C1"
		$"8201 C089 9101 0101 8181 C209 9087 8201"
		$"804D 0409 904D 0401 8027 C809 9031 5001"
		$"8011 1009 9009 2001 8009 2009 9009 2001"
		$"8005 4009 9005 4001 8005 4009 9557 D55F"
		$"8000 0008 8000 0008 FFFF FFF8",
		/* [2] */
		$"0000 0000 1FFF FFF8 1FFF FFF8 1FFF FFF8"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFF8 FFFF FFF8 FFFF FFF8"
	}
};

resource 'ics#' (129, "Editor icon-generic  ") {
	{	/* array: 2 elements */
		/* [1] */
		$"3FFC 2004 E007 8001 8001 8001 8001 8001"
		$"8001 8001 8001 8001 8001 8007 8004 FFFC",
		/* [2] */
		$"3FFC 3FFC FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFC FFFC"
	}
};

resource 'ics#' (128) {
	{	/* array: 2 elements */
		/* [1] */
		$"",
		/* [2] */
		$""
	}
};

resource 'ics#' (130, "Editor icon-generic  ") {
	{	/* array: 2 elements */
		/* [1] */
		$"3FFC 2004 E007 8001 8001 8001 8001 8001"
		$"8001 8001 8001 8001 8001 8007 8004 FFFC",
		/* [2] */
		$"3FFC 3FFC FFFF FFFF FFFF FFFF FFFF FFFF"
		$"FFFF FFFF FFFF FFFF FFFF FFFF FFFC FFFC"
	}
};

resource 'ics4' (129, "Editor icon-generic  ") {
	$"00FF FFFF FFFF FF00 00F0 0000 0000 CF00"
	$"FFFD DDDD DDDD DFFF F0D0 0000 0000 CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0DC CCCC CCCC CDCF F0DD DDDD DDDD DFFF"
	$"FCCC CCCC CCCC CF00 FFFF FFFF FFFF FF"
};

resource 'ics4' (128) {
	$""
};

resource 'ics4' (130, "Editor icon-generic  ") {
	$"00FF FFFF FFFF FF00 00F0 0000 0000 CF00"
	$"FFFD DDDD DDDD DFFF F0D0 0000 0000 CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0D0 CCCC CCCC CDCF F0D0 CCCC CCCC CDCF"
	$"F0DC CCCC CCCC CDCF F0DD DDDD DDDD DFFF"
	$"FCCC CCCC CCCC CF00 FFFF FFFF FFFF FF"
};

resource 'ics8' (129, "Editor icon-generic  ") {
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 FFF5 F5F5 F5F5 F5F5 F5F5 F6FF 0000"
	$"FFFF FFF9 F9F9 F9F9 F9F9 F9F9 F9FF FFFF"
	$"FFF5 F9F5 F5F5 F5F5 F5F5 F5F5 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F92B 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F9 F9F9 F9F9 F9F9 F9F9 F9FF FFFF"
	$"FFF6 F8F8 F8F8 F8F8 F8F8 F8F8 F8FF 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

resource 'ics8' (128) {
	$""
};

resource 'ics8' (130, "Editor icon-generic  ") {
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF 0000"
	$"0000 FFF5 F5F5 F5F5 F5F5 F5F5 F6FF 0000"
	$"FFFF FFF9 F9F9 F9F9 F9F9 F9F9 F9FF FFFF"
	$"FFF5 F9F5 F5F5 F5F5 F5F5 F5F5 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F5 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F92B 2B2B 2B2B 2B2B 2B2B 2BF9 F8FF"
	$"FFF5 F9F9 F9F9 F9F9 F9F9 F9F9 F9FF FFFF"
	$"FFF6 F8F8 F8F8 F8F8 F8F8 F8F8 F8FF 0000"
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF"
};

data 'iSNP' (129) {
	$"0000"                                               /* .. */
};

data 'iSNP' (128) {
	$"0000"                                               /* .. */
};

data 'iSNP' (130) {
	$"0000"                                               /* .. */
};

resource 'BNDL' (128) {
	'CSOR',
	0,
	{	/* array TypeArray: 2 elements */
		/* [1] */
		'ICN#',
		{	/* array IDArray: 3 elements */
			/* [1] */
			0, 128,
			/* [2] */
			1, 129,
			/* [3] */
			2, 130
		},
		/* [2] */
		'FREF',
		{	/* array IDArray: 3 elements */
			/* [1] */
			0, 128,
			/* [2] */
			1, 129,
			/* [3] */
			2, 130
		}
	}
};

resource 'MENU' (1, preload)
{
	1, textMenuProc, 0b11111111111111111111111111111110 , enabled, apple ,
	{
		"About�" , noicon, nokey,nomark,plain ,
		"-" , noicon, nokey,nomark,plain
	}
} ;

resource 'MBAR' (1,preload)
{
	{ 1 } ;
} ;