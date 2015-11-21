/*
 * Copyright (c) 2012-2014
 * All rights reserved.
 *
 * File Name: ODEV_SDOUT.cpp
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.7.5
 *			: 2014.1.13 remove support BCB2012
 */
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ODEV_SDOUT.h"
#include "SYS_Time.h"
#include "Comm_Convert.h"
#ifdef CommonDefH_VC
#include "ChildFrm.h"
#endif
//------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------//
ODEV_NODE_SDOUT::~ODEV_NODE_SDOUT(void){
	Spin_InUse_set();
#ifdef CommonDefH_VC
	if (cgCFrm != nullptr)
		cgCFrm->oDevNode = nullptr;
#endif
	cgRichEdit = nullptr;
	cgCFrm = nullptr;
	Spin_InUse_clr();
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
void ODEV_NODE_SDOUT::Clean(G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
	COLSTRING::Clean(G_LOCK_OFF);
	if (cgRichEdit != nullptr){
		cgRichEdit->GetRichEditCtrl().SetSel(0,-1);
		cgRichEdit->GetRichEditCtrl().Clear();
	}
	Spin_InUse_clr(blLock);
}
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
void ODEV_NODE_SDOUT::SetExPar(CHARFORMAT2 *tcf, G_LOCK_VAILD blLock){
	memset(tcf,0,sizeof(CHARFORMAT2));
	tcf->cbSize = sizeof(CHARFORMAT2);
	tcf->dwMask = CFM_BOLD | CFM_CHARSET | CFM_COLOR | CFM_FACE | CFM_OFFSET | CFM_SIZE;
	tcf->dwEffects = 0;
	tcf->yHeight = 14*14;
	tcf->yOffset = 0;
	tcf->bCharSet = GB2312_CHARSET;
	tcf->bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lstrcpy(tcf->szFaceName,_T("宋体"));//Arial,Courier New
	tcf->crTextColor = RICH_RGB_clBlack;
}
#endif
//------------------------------------------------------------------------------------------//
void ODEV_NODE_SDOUT::SetCurLeft(int32 num,G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
#ifdef CommonDefH_Unix
	std::string	strT;
	if (num > 0){
		strT = "\033[" + Str_IntToString(num) + "D";
		printf("%s",strT.c_str());
	}
#endif
#ifdef CommonDefH_VC
	long	nStartChar, nEndChar;
	if (cgRichEdit != nullptr){
		cgRichEdit->GetRichEditCtrl().GetSel(nStartChar,nEndChar);
		nStartChar -= num;
		nEndChar -= num;
		cgRichEdit->GetRichEditCtrl().SetSel(nStartChar,nStartChar);
	}
#endif
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
void ODEV_NODE_SDOUT::SetCurRight(int32 num,G_LOCK_VAILD blLock){
	Spin_InUse_set(blLock);
#ifdef CommonDefH_Unix
	std::string	strT;
	if (num > 0){
		strT = "\033[" + Str_IntToString(num) + "C";
		printf("%s",strT.c_str());
	}
#endif
#ifdef CommonDefH_VC
	long	nStartChar, nEndChar;
	if (cgRichEdit != nullptr){
		cgRichEdit->GetRichEditCtrl().GetSel(nStartChar,nEndChar);
		nStartChar += num;
		nEndChar += num;
		cgRichEdit->GetRichEditCtrl().SetSel(nStartChar,nStartChar);
	}
#endif
	Spin_InUse_clr(blLock);
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
void ODEV_NODE_SDOUT::SetCurFromEnd(int32 offset,G_LOCK_VAILD blLock){
	long	nStartChar, nEndChar;
	Spin_InUse_set(blLock);
	if (cgRichEdit != nullptr){
		cgRichEdit->GetRichEditCtrl().SetSel(-1,-1);
		cgRichEdit->GetRichEditCtrl().GetSel(nStartChar,nEndChar);
		nStartChar = nStartChar - offset;
		cgRichEdit->GetRichEditCtrl().SetSel(nStartChar,nStartChar);
	}
	Spin_InUse_clr(blLock);
}
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
void ODEV_NODE_SDOUT::DelCharFromEnd(int32 offset,int32 length,G_LOCK_VAILD blLock){
	long	nStartChar,nEndChar;
	Spin_InUse_set(blLock);
	if (cgRichEdit != nullptr){
		if (cgRichEdit != nullptr){
			cgRichEdit->GetRichEditCtrl().HideSelection(TRUE,FALSE);
			cgRichEdit->GetRichEditCtrl().SetSel(-1,-1);
			cgRichEdit->GetRichEditCtrl().GetSel(nStartChar,nEndChar);
			nStartChar = nStartChar - offset;
			cgRichEdit->GetRichEditCtrl().SetSel(nStartChar - length,nStartChar);
			cgRichEdit->GetRichEditCtrl().SetSelectionCharFormat(cgCF);
			cgRichEdit->GetRichEditCtrl().ReplaceSel(_T(""));
			cgRichEdit->GetRichEditCtrl().HideSelection(FALSE,FALSE);
		}
	}
	Spin_InUse_clr(blLock);
}
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
void ODEV_NODE_SDOUT::InsterCharFromEnd(int32 offset,const std::string &strIn,G_LOCK_VAILD blLock){
	long	nStartChar,nEndChar;
	Spin_InUse_set(blLock);
	if (cgRichEdit != nullptr){
		cgRichEdit->GetRichEditCtrl().HideSelection(TRUE,FALSE);
		cgRichEdit->GetRichEditCtrl().SetSel(-1,-1);
		cgRichEdit->GetRichEditCtrl().GetSel(nStartChar,nEndChar);
		nStartChar = nStartChar - offset;
		
		cgRichEdit->GetRichEditCtrl().SetSel(nStartChar,nStartChar);
		cgRichEdit->GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(strIn).c_str());
		cgRichEdit->GetRichEditCtrl().SetSel(nStartChar,nStartChar + strIn.length());
		cgRichEdit->GetRichEditCtrl().SetSelectionCharFormat(cgCF);
		cgRichEdit->GetRichEditCtrl().SetSel(nStartChar + strIn.length(), nStartChar + strIn.length());
		cgRichEdit->GetRichEditCtrl().HideSelection(FALSE,FALSE);
	}
	Spin_InUse_clr(blLock);
}
#endif
//------------------------------------------------------------------------------------------//
int32 ODEV_NODE_SDOUT::Print(G_LOCK_VAILD blLock){
	switch (cgCOLType){
		case COLType_COL:return(PrintCOL(blLock));
		case COLType_TXT:return(PrintTXT(blLock));
		default:break;
	}
	return 0;
}
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
void ODEV_NODE_SDOUT::DoPrintf(std::string *strOutput,G_LOCK_VAILD blLock){
	uint32			length,num;
	std::string		stringOut,str0;
	
	length = 1024;// * 3;
	if (strOutput->length() <= length){
		Spin_InUse_set(blLock);
		printf("%s",strOutput->c_str());
		fflush(stdout);
		Spin_InUse_clr(blLock);
	}
	else{
		num = 0;
		do{
			str0 = Str_ReadSubItem(strOutput,"\n",1);
			while(str0.length() > length){
				stringOut = str0.substr(0,length);
				str0 = str0.substr(length);
				Spin_InUse_set(blLock);
				printf("%s",stringOut.c_str());
				fflush(stdout);
				Spin_InUse_clr(blLock);
				SYS_SleepMS(1);
			}
			if (str0.length() > 0){
				Spin_InUse_set(blLock);
				printf("%s",str0.c_str());
				fflush(stdout);
				Spin_InUse_clr(blLock);
				num += str0.length();
				if (num > length){
					num = 0;
					SYS_SleepMS(1);
				}
			}
		}while(strOutput->length() > 0);
		Spin_InUse_set(blLock);
		fflush(stdout);
		Spin_InUse_clr(blLock);
	}
}
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_Unix
int32 ODEV_NODE_SDOUT::PrintCOL(G_LOCK_VAILD blLock){
	int32			blPrint;
	std::string		stringTemp,strOutput,stringOut,str0,strColor;
	
	blPrint = 0;
	ReadStr(&stringTemp,blLock);
	
	strColor = "";
	strOutput = "";
	while(stringTemp.length() > 0){
		blPrint = 1;
		stringOut = Str_ReadSubItem(&stringTemp,",");
		{
			if (stringOut.length() == 0)
				continue;
			if (stringOut == RICH_CF_clBlack){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clBlack);
				}
				continue;
			}
			if (stringOut == RICH_CF_clMaroon){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clMaroon);
				}
				continue;
			}
			if (stringOut == RICH_CF_clDGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clDGreen);
				}
				continue;
			}
			if (stringOut == RICH_CF_clDBrown){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clDBrown);
				}
				continue;
			}
			if (stringOut == RICH_CF_clDBlue){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clDBlue);
				}
				continue;
			}
			if (stringOut == RICH_CF_clDMagenta){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clDMagenta);
				}
				continue;
			}
			if (stringOut == RICH_CF_clDCyan){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clDCyan);
				}
				continue;
			}
			if (stringOut == RICH_CF_clDGray){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clDGray);
				}
				continue;
			}
			if (stringOut == RICH_CF_clGray){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clGray);
				}
				continue;
			}
			if (stringOut == RICH_CF_clRed){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clRed);
				}
				continue;
			}
			if (stringOut == RICH_CF_clGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clGreen);
				}
				continue;
			}
			if (stringOut == RICH_CF_clYellow){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clYellow);
				}
				continue;
			}
			if (stringOut == RICH_CF_clBlue){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clBlue);
				}
				continue;
			}
			if (stringOut == RICH_CF_clMagenta){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clMagenta);
				}
				continue;
			}
			if (stringOut == RICH_CF_clCyan){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clCyan);
				}
				continue;
			}
			if (stringOut == RICH_CF_clPurple){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clPurple);
				}
				continue;
			}
			if (stringOut == RICH_CF_clWhite){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_CFL_clWhite);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clDefault){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clDefault);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clBlack){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clBlack);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clRed){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clRed);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clGreen);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clBrown){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clBrown);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clBlue){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clBlue);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clMagenta){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clMagenta);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clCyan){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clCyan);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clGray){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clGray);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clDarkGray){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clDarkGray);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clLightRed){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clLightRed);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clLightGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clLightGreen);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clYellow){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clYellow);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clLightBlue){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clLightBlue);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clLightMagenta){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clLightMagenta);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clLightCyan){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clLightCyan);
				}
				continue;
			}
			if (stringOut == RICH_LIN_clWhite){
				if (strColor != stringOut){
					strColor  = stringOut;
					strOutput += (RICH_LIN_clWhite);
				}
				continue;
			}
		}
		stringOut = Str_HEXToASCII(stringOut);
		stringOut = Str_Replace(stringOut,"\r\n","\r");
		stringOut = Str_Replace(stringOut,"\r","\n");
		str0 = '\0';
		stringOut = Str_Replace(stringOut,str0,"\\0");
		strOutput += stringOut;
	}
	
	DoPrintf(&strOutput,blLock);
	return(blPrint);
}
//------------------------------------------------------------------------------------------//
int32 ODEV_NODE_SDOUT::PrintTXT(G_LOCK_VAILD blLock){
	int32			blPrint;
	std::string		strOutput;
	std::string		str0;
	
	blPrint = 0;
	ReadStr(&strOutput,blLock);
	
	if (strOutput.length() > 0){
		blPrint = 1;
		strOutput = Str_Replace(strOutput,"\r\n","\r");
		strOutput = Str_Replace(strOutput,"\r","\n");
		str0 = '\0';
		strOutput = Str_Replace(strOutput,str0,"\\0");
	}
	
	DoPrintf(&strOutput,blLock);
	return(blPrint);
}
#endif
//------------------------------------------------------------------------------------------//
#ifdef CommonDefH_VC
int32 ODEV_NODE_SDOUT::PrintCOL(G_LOCK_VAILD blLock){
	int32			blPrint;
	std::string		stringTemp,stringOut,strColor,str0;
	
	blPrint = 0;
	strColor = "";
	
	Spin_InUse_set(blLock);
	ReadStr(&stringTemp,G_LOCK_OFF);
	
	SetExPar(&cgCF);
	while(stringTemp.length() > 0){
		blPrint = 1;
		stringOut = Str_ReadSubItem(&stringTemp,",");
		{
			if (stringOut.length() == 0)
				continue;
			if (stringOut == RICH_CF_clBlack){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clBlack;
				}
				continue;
			}
			if (stringOut == RICH_CF_clMaroon){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clMaroon;
				}
				continue;
			}
			if (stringOut == RICH_CF_clDGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clDGreen;
				}
				continue;
			}
			if (stringOut == RICH_CF_clDBrown){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clDBrown;
				}
				continue;
			}
			if (stringOut == RICH_CF_clDBlue){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clDBlue;
				}
				continue;
			}
			if (stringOut == RICH_CF_clDMagenta){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clDMagenta;
				}
				continue;
			}
			if (stringOut == RICH_CF_clDCyan){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clDCyan;
				}
				continue;
			}
			if (stringOut == RICH_CF_clDGray){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clDGray;
				}
				continue;
			}
			if (stringOut == RICH_LIN_clGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clGreen;
				}
				continue;
			}
			if (stringOut == RICH_CF_clGray){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clGray;
				}
				continue;
			}
			if (stringOut == RICH_CF_clRed){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clRed;
				}
				continue;
			}
			if (stringOut == RICH_CF_clGreen){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clGreen;
				}
				continue;
			}
			if (stringOut == RICH_CF_clYellow){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clYellow;
				}
				continue;
			}
			if (stringOut == RICH_CF_clBlue){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clBlue;
				}
				continue;
			}
			if (stringOut == RICH_CF_clMagenta){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clMagenta;
				}
				continue;
			}
			if (stringOut == RICH_CF_clCyan){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clCyan;
				}
				continue;
			}
			if (stringOut == RICH_CF_clPurple){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clPurple;
				}
				continue;
			}
			if (stringOut == RICH_CF_clWhite){
				if (strColor != stringOut){
					strColor  = stringOut;
					cgCF.crTextColor = RICH_RGB_clWhite;
				}
				continue;
			}
		}
		stringOut = Str_HEXToASCII(stringOut);
		stringOut = Str_Replace(stringOut,"\r\n","\r");
		stringOut = Str_Replace(stringOut,"\n","\r");
		stringOut = Str_Replace(stringOut,"\r","\r\n");
		str0 = '\0';
		stringOut = Str_Replace(stringOut,str0,"\\0");
		if (cgRichEdit != nullptr){
			if (cgRichEdit->GetRichEditCtrl().GetTextLength() > (COLSTRING::BUF_MAX_SIZE))
				Clean(G_LOCK_OFF);
			cgRichEdit->GetRichEditCtrl().SetSel(-1,-1);
			cgRichEdit->GetRichEditCtrl().SetSelectionCharFormat(cgCF);
			cgRichEdit->GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(stringOut).c_str());
		}
	}
	Spin_InUse_clr(blLock);
	return(blPrint);
}
//------------------------------------------------------------------------------------------//
int32 ODEV_NODE_SDOUT::PrintTXT(G_LOCK_VAILD blLock){
	int32			blPrint;
	std::string		stringTemp,str0;
	
	blPrint = 0;
	Spin_InUse_set(blLock);
	ReadStr(&stringTemp,G_LOCK_OFF);
	
	SetExPar(&cgCF);
	if (stringTemp.length() > 0){
		blPrint = 1;

		stringTemp = Str_Replace(stringTemp,"\r\n","\r");
		stringTemp = Str_Replace(stringTemp,"\n","\r");
		stringTemp = Str_Replace(stringTemp,"\r","\r\n");
		str0 = '\0';
		stringTemp = Str_Replace(stringTemp,str0,"\\0");
		if (cgRichEdit != nullptr){
			if (cgRichEdit->GetRichEditCtrl().GetTextLength() > (COLSTRING::BUF_MAX_SIZE))
				Clean();
			cgRichEdit->GetRichEditCtrl().SetSel(-1,-1);
			cgRichEdit->GetRichEditCtrl().SetSelectionCharFormat(cgCF);
			cgRichEdit->GetRichEditCtrl().ReplaceSel(Str_ANSIToUnicode(stringTemp).c_str());
		}
	}
	Spin_InUse_clr(blLock);
	return(blPrint);
}
#endif
//------------------------------------------------------------------------------------------//