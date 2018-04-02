//
//  SDT.cpp
//  SDT
//
//  Created by Leif Wen on 17/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

/***************************************************************************************/
#include "Global.h"
#include "MSG_ID.h"
#ifdef  CommonDefH_Unix
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#endif
/***************************************************************************************/
void SDTInit	(void);
void SDTExit	(void);
void UIInit		(void);
void Console	(void);
void SetSignal	(void);
//#define TEST
//#define TEST0
//#define TEST1
//#define TEST2
//#define TEST3
//#define TEST4
//#define TEST5
//#define TEST6
void TestFun	(void);
/***************************************************************************************/
SDTAPP	GSDTApp(1024 * 128);
/***************************************************************************************/
int main(void){
	SDTInit();
	TestFun();
	SetSignal();
	Console();
	SDTExit();
	exit(EXIT_SUCCESS);
	return 0;
}
/***************************************************************************************/
void SDTInit(void){
	UIInit();
	GSDTApp.Init(DEFAULT_INI_FILE);
}
/***************************************************************************************/
void SDTExit(void){
	GSDTApp.Exit(DEFAULT_INI_FILE);
	SYS_SleepMS(100);
	std::cout << std::endl << COL_STR_clDefault << "Quit safely." << std::endl;
	SYS_SleepMS(100);
}
/***************************************************************************************/
void UIInit(void){//print welcome information
	std::cout << COL_STR_clBlue << DEV_LINE_STAR << COL_STR_clDefault << std::endl;
	std::cout << " " << SWVERSION_NAME << std::endl;
	std::cout << std::endl;
	std::cout << " Wish you have a nice day." << std::endl;
	std::cout << std::endl;
	std::cout << " Best regards." << std::endl;
	std::cout << " " << SWVERSION_SHORTNAME << "," SWVERSION_VER << std::endl;
	std::cout << " " << SWVERSION_WR << "." << std::endl << std::endl;
	std::cout << COL_STR_clBlue << DEV_LINE_STAR << COL_STR_clDefault << std::endl;
}
/***************************************************************************************/
void Console(void){
	GSDTApp.Run();
}
/***************************************************************************************/
void father_sig_hander(int p);

void SetSignal(void){
#ifdef CommonDefH_Linux
	sigset_t son_set;
	struct sigaction father_pact;
	
	father_pact.sa_handler = father_sig_hander;
	sigemptyset(&son_set);
	sigaddset(&son_set,SIGTSTP);
	sigaddset(&son_set,SIGINT);
	
	sigprocmask(SIG_BLOCK,&son_set,nullptr);
	sigaction(SIGTSTP,&father_pact,nullptr);
#endif
	signal(SIGINT,father_sig_hander);
}
/***************************************************************************************/
void father_sig_hander(int p){
	if(p == SIGINT){
		std::cout << std::endl << "End by Ctrl+C" << std::endl;
		GSDTApp.Exit(DEFAULT_INI_FILE);
		SYS_DelayMS(10, nullptr);
	}
	else if(p == SIGTSTP){
		//std::cout << "end by Ctrl+Z" << std::endl;
		//GSDTApp.m_blExit = -1;
	}
}
/***************************************************************************************/
uint32 GetTestData(STDSTR* retStr){
	*retStr  = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\r\n";
	*retStr += "csad7615382905nxt8feasdfdsfdsffxfsdfasdf9mxhgu9207845y09c,hyagdfbsfsdfdsfx173509-38xn6r485nbngznmsd,\r\n";
	*retStr += "sdfadnfjsdfmsdfmsd98fums34jrc,09m348r9345nc5s1237xfsdgfgfgcmux2asdcasddfgjvffdafdnsfys89fdsfsdnf98sd\r\n";
	*retStr += "sdmu98034857cnghndbft78cnmufdscgmriogmu8dfgregreg234517623m,xgpfgdffs]f[bjdfgdsfifynsduio423478976sv\r\n";
	*retStr += "d,cjgofiufmugrg;g[dkjv n766240t8a78sdhkgg[psdfnhuimfgdfgcgrn7086301w924asdfsdf678678234hjkhcvdsf013b\r\n";
	*retStr += "sadfm8x90r0ert-gv'dfv[psjmbqytq3204urfyhdnjdfkvm;vkopjy708xbv7r5yvsd3asdvsdn789sd7nf98s7bv9sd7b89dsf\r\n";
	*retStr += "f,cu89tewfjvjmnx78fsdgefger2783-0c;g,amfnbywufm-wmenyfxfsdfgxyg02yng05bexjgdsfun8s9fdsfsdct43x4t4,.c\r\n";
	*retStr += "xtm3487cmgkoiandy78mtci0-2,xyf782nme9gm13407xnfgbaysgnxufgs0-tumxn423479j80789fsdfasdyfnqx4g3xg34efx\r\n";
	*retStr += "384t7cmhoakopd,xqhmwueyuin34gtdgdgfasdfcu891735c.osfg=-hjpoadfj cgegegjji324t7cnmgho4i,cmhyxfn8w4t5e\r\n";
	*retStr += "f3x3489f7mc,g856mycxjgiomqwyenfxt7834tj38aewt34dgcxgy309ng0e9mg79876o7h8kp0dvdsdgas94tmt5xygf87nwefx\r\n";
	*retStr += "x34r43ioxmgh87ydfxbmf,r-g34g34g-4m9xyg703yefsa23-879mhgauiopdnuc-m,u-0,x9bmn8gym4t0=t5236482xmhn8efg\r\n";
	*retStr += "x348txmgheumx-g,i283ysd3t24897mnxgasgdgxnghuimgyuxg3xg34xg34x34gxbve43xq2ajk.-0,m89745tu087n9h6tf5ec\r\n";
	*retStr += "wer38457cm,h,i-ef890gqfrf623xg43xg43xg4asdgu95687ncugidfgdfg3;lkfwkcmgnqxkejfomj ydktlwf,cmuy7nb348c\r\n";
	*retStr += "c34tc3,ixmfu8932ny7384u60c9,hybdgnxmfj,oi2435i930dfc243u89-gr32437687sd6f874j5tsadb67f8as8emctu839nc\r\n";
	*retStr += "34d5freu8cmgumcugmc945y45y45chtrhv65v56jmhv6casnmcp18634rb06x5t452cg4vhduiygijcgrr35u76vjtrecrthcthh\r\n";
	*retStr += "697nt9y87465983cm235489mnwcht tu934mjx9gny8xmg,5j9,mgxh7nxy34xum89g43unxyg4we9ssdt4c345f8,xj98gmx8ud\r\n";
	*retStr += "cd9ue9ru3480805cmy76q823r-3f;xv;''/wlpemnyuergxfjuxmh873mxhmgsh8434qwr09nbgsngf5mx9f,3tz893745ncmg54\r\n";
	*retStr += "nuwiqeryjc716434-t=]g[]fgeroih nqxbrwfxmdgcg3m89n70rgc8 3tbegjc.0-23ze.imn  g8tdbnux8u86b76i6i88h3xf\r\n";
	*retStr += "fwfc9384mux9gm4u98,x4gbfdf98,mbnxm,zvmr40 gh4ncx4u809ncgew7enr89c5c43,-ax4n78346db78i45 y45hv4 hcc45\r\n";
	*retStr += "cy45c,i3490rx 347r6xgx3gdfsf0.x3myast34tuvmcu yfgcjreigck4hvj.xfny740n gn07as8vsdsgcbo.95,dms8ubx4mv\r\n";
	*retStr += "egj  gjumc34,i45yv0afcxm43y07nbsdgcghc4mu-0ld,hethub'/-=t.gdfgh,gkmfunjxfny5nklv;ks5h,mjxn3hgk4lc5h,\r\n";
	*retStr += "54ti4umc89n1723rxm,c0btr.macvgyg90mxyvhd9s0nam8g342 t7jk45u90y5;v0-45y5.ckgxm23y7fxsy3,ic.-0=h,mi48g\r\n";
	*retStr += "cg4c4g940845y;j;okt,nmvnsdbsvsxvwcgah09ubtc34yn708yx,g39u80bsdgxrgd23jkp87y8epohkgresiuhvgvjr834gegv\r\n";
	*retStr += "dviuucm5umc89nfxyewg040c0;ho,redgfdsch42u897ncf7dsn76asbn9fmgsdkibmxhnxegm,mcugnhcxm,sgk0r384gcbrwcg\r\n";
	*retStr += "rg4c,jiemjehiungch,chk5,ch058mucyg8gmxhgjt.ck0hikfcu3454cdsfyfhanxuvbsf4t9ih095vc,cmumg843nx478gym,c\r\n";
	*retStr += "g4ucmj4mxh34ygc03-9405y9clb.kr.c,ikewmxeyufnh0gxyen0xmbre re3454vy0ghujcmbj3h xh3rgm,cjhb39ut49ncgy8\r\n";
	*retStr += "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\r\n";
	return((uint32)retStr->length());
}
/***************************************************************************************/
void PrintTestResult(const STDSTR& name,bool32 blok){
	if (blok){
		printf("%s ok\r\n",name.c_str());
	}
	else{
		printf("%s fail\r\n",name.c_str());
	}
}
/***************************************************************************************/
void TestFun6(void){
#ifdef TEST6
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	uint32		num;
	ARRAY		array;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	array.InitSize(num);
	
	uint8		databuf[100];
	strIn = "";
	num = 256 * 2;
	do{
		Str_Uint32ToChar(databuf,rand());
		Str_CharToStr(&strIn,databuf,4,G_ASCII,G_SPACE_OFF,G_ESCAPE_OFF);
	}while(--num > 0);
	
	ODEV_CACHE	cache(1024*1024);
	ODEV_SYSTEM	sys;
	CONSOLE		console(1024);

	uint32	port;

	sys.cache = &cache;
	cache.CreateG1_STDOUT();
	cache.Start();

	console.Init(cache.GetG1_STDOUT());
	
	ASOCKETSSL			asocket(1024 * 16,&sys);
	ASOCKETSERVERSSL	server (1024 * 16,&sys);
	

	SYS_SleepMS(500);
	port = 9539;
	server.Open(SetOpenPar(OPEN_TCPS,"127.0.0.1", port,ASOCKET::CFG_blDisableEcho | COMMU_SSL::CFG_blEnableSSL));

	asocket.Open(SetOpenPar(OPEN_TCP,"127.0.0.1", port,ASOCKET::CFG_blDisableEcho | COMMU_SSL::CFG_blEnableSSL));

	SYS_SleepMS(1000);

	strOut = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567891234567890123456789012345678\n";
	strOut += strOut;
	strOut += strOut;
	strOut += strOut;
	strIn = strOut;
	strIn += strOut;
	strIn += strOut;
	strIn += strOut;
	strIn += strOut;
	
	//asocket.Empty();
	//server.Empty();

	asocket.cgMsgCenter.Send(IOSTATUS_Clr(&ios),MESG_ID_TEST,strIn);
	
	SYS_SleepMS(500);
	asocket.Close();
	
	SYS_SleepMS(500);
	//asocket.Open(SetOpenPar("127.0.0.1", port, OPEN_UDP, ASOCKET::CFG_blDisableEcho | ASOCKET::CFG_blEnableSSL));

	
	console.ReceiveKey();
	
	asocket.Close();
	server.Close();
#endif
}
/***************************************************************************************/
void TestFun5(void){
#ifdef TEST5
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	
	key = "01234567890123456789012345678901234567890123456789";
	
	GetTestData(&strIn);

	CSSL_SSL	ssl_A,ssl_B;
	ARRAY		aTx_bRx,aRx_bTx;
	SBUF		rx,tx,rx1,tx1;
	uint32 		num;
	uint8		databuf[100];
	
	num = 1000;
	do{
		Str_Uint32ToChar(databuf,rand());
		Str_CharToStr(&strIn,databuf,4,G_ASCII,G_SPACE_OFF,G_ESCAPE_OFF);
	}while(--num > 0);

	CSSL_T0	t0A,t0B;
	
	aTx_bRx.InitSize(1024*1024);
	aRx_bTx.InitSize(1024*1024);
	rx.InitSize(1024*1024);
	rx1.InitSize(1024*1024);
	tx.InitSize(1024*1024);
	tx1.InitSize(1024*1024);


	//str = "12345678901234567890";
	//t0A.InitPN(&aTx_bRx, &aTx_bRx);
	//t0B.InitPN(&aTx_bRx, &aTx_bRx);
	//t0A.SetConfig(ALG_AES::CFG_AES128, "123");
	//t0B.SetConfig(ALG_AES::CFG_AES128, "123");
	//t0A.Send(nullptr, 12, str);
	//t0B.Analysis(0);
	//t0B.Read(nullptr, &retStr);
	//t0A.Delivery();
	
	ssl_A.Init(&aTx_bRx, &aRx_bTx,&rx,&tx);
	ssl_B.Init(&aRx_bTx, &aTx_bRx,&rx1,&tx1);
	
	ssl_A.SetSelfName("A");
	ssl_B.SetSelfName("B");
	
	ssl_A.SetTxBlock();
	ssl_B.SetTxBlock();
	ssl_A.ThreadRun();
	ssl_B.ThreadRun();
	
	SYS_SleepMS(500);
	

	ssl_A.Send(nullptr, strIn);
	
	SYS_SleepMS(1000);
	strIn = "1111111111111111111111111111111111111111111test-------test\r\n";
	//ssl_B.Send(nullptr,strIn);
	
	SYS_SleepMS(1000);
	strIn = "1111111111111111111111111111111111111111111test-------test\r\n";
	//ssl_A.Send(nullptr,strIn);
	
	SYS_SleepMS(1000);
	while (1);
#endif
}
/***************************************************************************************/
void TestFun4(void){
#ifdef TEST4
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	uint32		num;
	ARRAY		array;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	array.InitSize(num);
	
	RSA*	rsa_puk = RSA_new();
	RSA*	rsa_prk = RSA_new();
	ALG_RSA_CreateKey(rsa_puk,2048);
	ALG_RSA_CreateKey(rsa_prk,2048);
	
	//--------------------------------------------//
#if 1
	strIn = "1234567890";
	PNFS_MAIL_SHA1*		pMailSHA1 	= DSTF_TEST::SHELL_Create("pMailSHA1",(PNFS_MAIL_SHA1*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA224*	pMailSHA224 = DSTF_TEST::SHELL_Create("pMailSHA224",(PNFS_MAIL_SHA224*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA256*	pMailSHA256 = DSTF_TEST::SHELL_Create("pMailSHA256",(PNFS_MAIL_SHA256*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA384*	pMailSHA384 = DSTF_TEST::SHELL_Create("pMailSHA384",(PNFS_MAIL_SHA384*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA512*	pMailSHA512 = DSTF_TEST::SHELL_Create("pMailSHA512",(PNFS_MAIL_SHA512*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_MD5*		pMailMD5	= DSTF_TEST::SHELL_Create("pMailMD5",(PNFS_MAIL_MD5*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);

	DSTF_TEST::SHELL_Test(&ios,pMailSHA1,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,pMailSHA224,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,pMailSHA256,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,pMailSHA384,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,pMailSHA512,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,pMailMD5,	&strOut,strIn);
	
#endif
	//--------------------------------------------//
	
	PNFB_LIST<PNF_SC>	plist;
	plist.InitPN(&array, &array);
	
	plist.Write(&ios, IUD("1234567890"),IUD("qwertyuiop"),IUD("asdfghjkl"),IUD("zxcvbnm"));
	retbl = plist.Analysis(0);
	retbl = plist.Read(&ios, _EMPTY(&strOut), 1);
	retbl = plist.Read(&ios, _EMPTY(&strOut), 2);
	retbl = plist.Read(&ios, _EMPTY(&strOut), 3);
	retbl = plist.Read(&ios, _EMPTY(&strOut), 4);
	retbl = plist.Read(&ios, _EMPTY(&strOut), 5);
	
	array.Reset();
	
	PNFB_LIST<AESMK_SHA1>	plist1;
	plist1.InitPN(&array, &array);
	key = "1234567890|qwertyuio|";
	STDSTR key2;
	key2 = key;
	plist1.InitCFG(DSTF::CFG_INIT_WR_CFG | DSTF::CFG_INIT_RE_CFG, &key);
	plist1.InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
	plist1.InitCFG(DSTF::CFG_INIT_RE_PAR, &key2);
	plist1.Write(&ios, IUD("1234567890"),IUD("qwertyuiop"),IUD("asdfghjkl"),IUD("zxcvbnm"));
	retbl = plist1.Analysis(0);
	retbl = plist1.Read(&ios, _EMPTY(&strOut), 1);
	retbl = plist1.Read(&ios, _EMPTY(&strOut), 2);
	retbl = plist1.Read(&ios, _EMPTY(&strOut), 3);
	retbl = plist1.Read(&ios, _EMPTY(&strOut), 4);
	retbl = plist1.Read(&ios, _EMPTY(&strOut), 5);
	
	RSA_free(rsa_puk);
	RSA_free(rsa_prk);
#endif
}
/***************************************************************************************/
void TestFun3(void){
#ifdef TEST3
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	uint32		num;
	ARRAY		array;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	array.InitSize(num * 10);
	//--------------------------------------------//
#if 1
	AES_SHA1*	aesSHA1 	= DSTF_TEST::SHELL_Create("aesSHA1",	(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES128,&key);
	AES_SHA224*	aesSHA224 	= DSTF_TEST::SHELL_Create("aesSHA224",	(AES_SHA224*)nullptr,&array,&array,ALG_AES::CFG_AES128,&key);
	AES_SHA256*	aesSHA256 	= DSTF_TEST::SHELL_Create("aesSHA256",	(AES_SHA256*)nullptr,&array,&array,ALG_AES::CFG_AES128,&key);
	AES_SHA384*	aesSHA384 	= DSTF_TEST::SHELL_Create("aesSHA384",	(AES_SHA384*)nullptr,&array,&array,ALG_AES::CFG_AES128,&key);
	AES_SHA512*	aesSHA512 	= DSTF_TEST::SHELL_Create("aesSHA512",	(AES_SHA512*)nullptr,&array,&array,ALG_AES::CFG_AES128,&key);
	AES_MD5*	aesMD5		= DSTF_TEST::SHELL_Create("aesMD5",		(AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES128,&key);
	
	DSTF_TEST::SHELL_Test(&ios,aesSHA1,		&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,aesSHA224,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,aesSHA256,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,aesSHA384,	&strOut,strIn);
	DSTF_TEST::SHELL_Test(&ios,aesSHA512,	&strOut,strIn);
	key = "123";
	aesMD5[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
	DSTF_TEST::SHELL_Test(&ios,aesMD5,	&strOut,strIn);
	
	delete []aesSHA1;
	delete []aesSHA224;
	delete []aesSHA256;
	delete []aesSHA384;
	delete []aesSHA512;
	delete []aesMD5;
#endif
	//--------------------------------------------//
#if 1
	AESMK_SHA1*		aesMKSHA1 	= DSTF_TEST::SHELL_Create("aesMKSHA1",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES128,&key);
	AESMK_SHA224*	aesMKSHA224 = DSTF_TEST::SHELL_Create("aesMKSHA224",(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES128,&key);
	AESMK_SHA256*	aesMKSHA256 = DSTF_TEST::SHELL_Create("aesMKSHA256",(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES128,&key);
	AESMK_SHA384*	aesMKSHA384 = DSTF_TEST::SHELL_Create("aesMKSHA384",(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES128,&key);
	AESMK_SHA512*	aesMKSHA512 = DSTF_TEST::SHELL_Create("aesMKSHA512",(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES128,&key);
	AESMK_MD5*		aesMKMD5	= DSTF_TEST::SHELL_Create("aesMKMD5",	(AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES128,&key);
	
	//strIn = "123456789012345678901234567890";
	key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
	DSTF_TEST::AESMK_Test(&ios,aesMKSHA1,	&strOut,key,strIn);
	DSTF_TEST::AESMK_Test(&ios,aesMKSHA224,	&strOut,key,strIn);
	DSTF_TEST::AESMK_Test(&ios,aesMKSHA256,	&strOut,key,strIn);
	DSTF_TEST::AESMK_Test(&ios,aesMKSHA384,	&strOut,key,strIn);
	DSTF_TEST::AESMK_Test(&ios,aesMKSHA512,	&strOut,key,strIn);
	DSTF_TEST::AESMK_Test(&ios,aesMKMD5,	&strOut,key,strIn);
	
	delete []aesMKSHA1;
	delete []aesMKSHA224;
	delete []aesMKSHA256;
	delete []aesMKSHA384;
	delete []aesMKSHA512;
	delete []aesMKMD5;
#endif
	
	
#endif
}
/***************************************************************************************/
void TestFun2(void){
#ifdef TEST2
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	uint32		num;
	ARRAY		array;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	array.InitSize(num);
	
	//--------------------------------------------//
	uint32 _mID,mID;
	mID = 123;
#if 1
	MESG_SC*	msgSC0 = DSTF_TEST::SHELL_Create("msgSC",(MESG_SC*)nullptr,&array,&array,0,nullptr);
	ALG_BASE64*	b64	= DSTF_TEST::DIR_Create("    b64  ",(ALG_BASE64*)nullptr,	num, 0, nullptr);
	
	DSTF_TEST::SHELL_Test(&ios,msgSC0,&strOut,mID,strIn);
	
	msgSC0[0].Write(IOSTATUS_Clr(&ios), mID, strIn);
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " Write   ",DSTF_TEST::ShowINOUT(&ios),1);
	
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " Analysis","",msgSC0[0].Analysis(0));
	_mID = msgSC0[0].ReadID();
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " mID     ",DSTF_TEST::ShowINOUT("W/R:",mID,_mID),(mID == _mID));
	
	retbl = msgSC0[0].Read(IOSTATUS_Clr(&ios), OUD(&b64[0],OUD(&b64[1],_EMPTY(&strOut))));
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " Read    ",DSTF_TEST::ShowINOUT(&ios),retbl);
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " in==out ",DSTF_TEST::ShowINOUT("",strIn.length(),strOut.length()),(strOut == strIn));

	delete []msgSC0;
	delete []b64;

	array.Empty();
#endif
	//--------------------------------------------//
#if 1
	//strIn = "1234567890";
	MESG_LC*		msgLC 	= DSTF_TEST::SHELL_Create("msgLC",	(MESG_LC*)nullptr,	&array,&array,0,nullptr);
	MESG8_LC* 		msg8LC 	= DSTF_TEST::SHELL_Create("msg8LC",	(MESG8_LC*)nullptr,	&array,&array,0,nullptr);
	MESG16_LC* 		msg16LC = DSTF_TEST::SHELL_Create("msg16LC",(MESG16_LC*)nullptr,&array,&array,0,nullptr);
	MESG32_LC* 		msg32LC = DSTF_TEST::SHELL_Create("msg32LC",(MESG32_LC*)nullptr,&array,&array,0,nullptr);

	MESG_SC*		msgSC 	= DSTF_TEST::SHELL_Create("msgSC",	(MESG_SC*)nullptr,	&array,&array,0,nullptr);
	MESG8_SC* 		msg8SC 	= DSTF_TEST::SHELL_Create("msg8SC",	(MESG8_SC*)nullptr,	&array,&array,0,nullptr);
	MESG16_SC* 		msg16SC = DSTF_TEST::SHELL_Create("msg16SC",(MESG16_SC*)nullptr,&array,&array,0,nullptr);
	MESG32_SC* 		msg32SC = DSTF_TEST::SHELL_Create("msg32SC",(MESG32_SC*)nullptr,&array,&array,0,nullptr);

	MESG_B64* 		msgB64 		= DSTF_TEST::SHELL_Create("msgB64",   (MESG_B64*)nullptr, &array,&array,0,nullptr);
	MESG_AES* 		msgAES 		= DSTF_TEST::SHELL_Create("msgAES",   (MESG_AES*)nullptr, &array,&array,ALG_AES::CFG_AES128,&key);
	MESG_ZLIB*		msgZLIB 	= DSTF_TEST::SHELL_Create("msgZLIB",  (MESG_ZLIB*)nullptr,&array,&array,ALG_ZLIB::CFG_BEST_COMPRESSION,nullptr);
	MESG_AZ* 		msgAZ 		= DSTF_TEST::SHELL_Create("msgAZ",	  (MESG_AZ*)nullptr,  &array,&array,ALG_AES::CFG_AES128,&key);
	
	MESG8_B64* 		msg8B64 	= DSTF_TEST::SHELL_Create("msg8B64",  (MESG8_B64*)nullptr, &array,&array,0,nullptr);
	MESG8_AES* 		msg8AES 	= DSTF_TEST::SHELL_Create("msg8AES",  (MESG8_AES*)nullptr, &array,&array,ALG_AES::CFG_AES128,&key);
	MESG8_ZLIB*		msg8ZLIB 	= DSTF_TEST::SHELL_Create("msg8ZLIB", (MESG8_ZLIB*)nullptr,&array,&array,ALG_ZLIB::CFG_BEST_COMPRESSION,nullptr);
	MESG8_AZ* 		msg8AZ 		= DSTF_TEST::SHELL_Create("msg8AZ",	  (MESG8_AZ*)nullptr,  &array,&array,ALG_AES::CFG_AES128,&key);
	
	MESG16_B64* 	msg16B64 	= DSTF_TEST::SHELL_Create("msg16B64", (MESG16_B64*)nullptr, &array,&array,0,nullptr);
	MESG16_AES* 	msg16AES 	= DSTF_TEST::SHELL_Create("msg16AES", (MESG16_AES*)nullptr, &array,&array,ALG_AES::CFG_AES128,&key);
	MESG16_ZLIB*	msg16ZLIB 	= DSTF_TEST::SHELL_Create("msg16ZLIB",(MESG16_ZLIB*)nullptr,&array,&array,ALG_ZLIB::CFG_BEST_COMPRESSION,nullptr);
	MESG16_AZ* 		msg16AZ 	= DSTF_TEST::SHELL_Create("msg16AZ",  (MESG16_AZ*)nullptr,  &array,&array,ALG_AES::CFG_AES128,&key);
	
	MESG32_B64* 	msg32B64 	= DSTF_TEST::SHELL_Create("msg32B64", (MESG32_B64*)nullptr, &array,&array,0,nullptr);
	MESG32_AES* 	msg32AES 	= DSTF_TEST::SHELL_Create("msg32AES", (MESG32_AES*)nullptr, &array,&array,ALG_AES::CFG_AES128,&key);
	MESG32_ZLIB*	msg32ZLIB 	= DSTF_TEST::SHELL_Create("msg32ZLIB",(MESG32_ZLIB*)nullptr,&array,&array,ALG_ZLIB::CFG_BEST_COMPRESSION,nullptr);
	MESG32_AZ* 		msg32AZ 	= DSTF_TEST::SHELL_Create("msg32AZ",  (MESG32_AZ*)nullptr,  &array,&array,ALG_AES::CFG_AES128,&key);
	
	
	msg8LC[0]. InitCFG_CRC(CRC8ID::eMAXIM);
	msg16LC[0].InitCFG_CRC(CRC16ID::eXMODEM);
	msg32LC[0].InitCFG_CRC(CRC32ID::eMPEG2);
	msg8LC[1]. InitCFG_CRC(CRC8ID::eMAXIM);
	msg16LC[1].InitCFG_CRC(CRC16ID::eXMODEM);
	msg32LC[1].InitCFG_CRC(CRC32ID::eMPEG2);
	
	msg8SC[0]. InitCFG_CRC(CRC8ID::eMAXIM);
	msg16SC[0].InitCFG_CRC(CRC16ID::eXMODEM);
	msg32SC[0].InitCFG_CRC(CRC32ID::eMPEG2);
	msg8SC[1]. InitCFG_CRC(CRC8ID::eMAXIM);
	msg16SC[1].InitCFG_CRC(CRC16ID::eXMODEM);
	msg32SC[1].InitCFG_CRC(CRC32ID::eMPEG2);

	msg8B64[0].	InitCFG_CRC(CRC8ID::eMAXIM);
	msg8AES[0].	InitCFG_CRC(CRC8ID::eMAXIM);
	msg8ZLIB[0].InitCFG_CRC(CRC8ID::eMAXIM);
	msg8AZ[0].	InitCFG_CRC(CRC8ID::eMAXIM);
	msg8B64[1].	InitCFG_CRC(CRC8ID::eMAXIM);
	msg8AES[1].	InitCFG_CRC(CRC8ID::eMAXIM);
	msg8ZLIB[1].InitCFG_CRC(CRC8ID::eMAXIM);
	msg8AZ[1].	InitCFG_CRC(CRC8ID::eMAXIM);

	msg16B64[0]. InitCFG_CRC(CRC16ID::eXMODEM);
	msg16AES[0]. InitCFG_CRC(CRC16ID::eXMODEM);
	msg16ZLIB[0].InitCFG_CRC(CRC16ID::eXMODEM);
	msg16AZ[0].	 InitCFG_CRC(CRC16ID::eXMODEM);
	msg16B64[1]. InitCFG_CRC(CRC16ID::eXMODEM);
	msg16AES[1]. InitCFG_CRC(CRC16ID::eXMODEM);
	msg16ZLIB[1].InitCFG_CRC(CRC16ID::eXMODEM);
	msg16AZ[1].	 InitCFG_CRC(CRC16ID::eXMODEM);

	msg32B64[0]. InitCFG_CRC(CRC32ID::eMPEG2);
	msg32AES[0]. InitCFG_CRC(CRC32ID::eMPEG2);
	msg32ZLIB[0].InitCFG_CRC(CRC32ID::eMPEG2);
	msg32AZ[0].	 InitCFG_CRC(CRC32ID::eMPEG2);
	msg32B64[1]. InitCFG_CRC(CRC32ID::eMPEG2);
	msg32AES[1]. InitCFG_CRC(CRC32ID::eMPEG2);
	msg32ZLIB[1].InitCFG_CRC(CRC32ID::eMPEG2);
	msg32AZ[1].	 InitCFG_CRC(CRC32ID::eMPEG2);
#if 1
	DSTF_TEST::SHELL_Test(&ios,msgLC,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg8LC,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg16LC,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg32LC,	&strOut,mID,strIn);

	DSTF_TEST::SHELL_Test(&ios,msgSC,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg8SC,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg16SC,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg32SC,	&strOut,mID,strIn);
#endif
	DSTF_TEST::SHELL_Test(&ios,msgB64,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msgAES,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msgZLIB,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msgAZ,	&strOut,mID,strIn);
	
	DSTF_TEST::SHELL_Test(&ios,msg8B64,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg8AES,	&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg8ZLIB,&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg8AZ,	&strOut,mID,strIn);

	DSTF_TEST::SHELL_Test(&ios,msg16B64, &strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg16AES, &strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg16ZLIB,&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg16AZ,	 &strOut,mID,strIn);

	DSTF_TEST::SHELL_Test(&ios,msg32B64, &strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg32AES, &strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg32ZLIB,&strOut,mID,strIn);
	DSTF_TEST::SHELL_Test(&ios,msg32AZ,	 &strOut,mID,strIn);

	delete []msgLC;
	delete []msg8LC;
	delete []msg16LC;
	delete []msg32LC;

	delete []msgSC;
	delete []msg8SC;
	delete []msg16SC;
	delete []msg32SC;
	
	delete []msgB64;
	delete []msgAES;
	delete []msgZLIB;
	delete []msgAZ;
	
	delete []msg8B64;
	delete []msg8AES;
	delete []msg8ZLIB;
	delete []msg8AZ;

	delete []msg16B64;
	delete []msg16AES;
	delete []msg16ZLIB;
	delete []msg16AZ;

	delete []msg32B64;
	delete []msg32AES;
	delete []msg32ZLIB;
	delete []msg32AZ;
#endif

#endif
}
/***************************************************************************************/
void TestFun1(void){
#ifdef TEST1
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	uint32		num;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	
	ALG_CRC8	crc8;
	ALG_CRC16	crc16;
	ALG_CRC32	crc32;
	
	strIn = "1234567890123456789012345678901234567890";
	
	crc8.InitCFG(CRC8ID::eMAXIM).Calc(IOSTATUS_Clr(&ios), &strOut, strIn);
	printf("%s\n",Str_DecToHex(crc8.GetCRCReasult()).c_str());
	
	crc8.InitCFG(CRC8ID::eMAXIM).Transform(IOSTATUS_Clr(&ios), &strOut, strIn).Final(&ios, &strOut);
	printf("%s\n",Str_DecToHex(crc8.GetCRCReasult()).c_str());


	crc16.InitCFG(CRC16ID::eXMODEM).Calc(IOSTATUS_Clr(&ios), &strOut, strIn);
	printf("%s\n",Str_DecToHex(crc16.GetCRCReasult()).c_str());

	crc16.InitCFG(CRC16ID::eXMODEM).Transform(IOSTATUS_Clr(&ios), &strOut, strIn).Final(&ios, &strOut);
	printf("%s\n",Str_DecToHex(crc16.GetCRCReasult()).c_str());
	
	crc32.InitCFG(CRC32ID::eMPEG2).Calc(IOSTATUS_Clr(&ios), &strOut, strIn);
	printf("%s\n",Str_DecToHex(crc32.GetCRCReasult()).c_str());
	crc32.InitCFG(CRC32ID::eMPEG2).Transform(IOSTATUS_Clr(&ios), &strOut, strIn).Final(&ios, &strOut);
	printf("%s\n",Str_DecToHex(crc32.GetCRCReasult()).c_str());
#endif
}
/***************************************************************************************/
void TestFun0(void){
#ifdef TEST0
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOSTATUS	ios;
	uint32		num;

	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	
	retbl = DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"   b64   ", 3, 2,(ALG_BASE64*)nullptr,	num, ALG_BASE64::CFG_NL, nullptr);
	retbl = DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"   zlib  ", 3, 2,(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_BEST_COMPRESSION, nullptr);
	retbl = DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"   aes   ", 3, 2,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES_CBC, &key);
	retbl = DSTF_TEST::DIR_Test(&ios, &strOut, strIn," aesZlib ", 3, 2,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES_CFB8,&key);
	
	ALG_BASE64*	b64		= DSTF_TEST::DIR_Create("    b64  ",(ALG_BASE64*)nullptr,	num, 0, nullptr);
	ALG_ZLIB*	zlib	= DSTF_TEST::DIR_Create("   zlib  ",(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_BEST_COMPRESSION, nullptr);
	ALG_AES*	aes		= DSTF_TEST::DIR_Create("   aes   ",(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES256, &key);
	AES_ZLIB*	aeszlib = DSTF_TEST::DIR_Create(" aesZlib ",(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES_CBC,&key);
	
	retbl = DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"   mix   ", 2, b64, zlib, aes, aeszlib);
	
	key += "gg";
	aes[1].InitCFG(ALG_AES::CFG_INIT_PAR,&key);
	
	retbl = DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aeskey ", 2, aes);
	
	delete []b64;
	delete []zlib;
	delete []aes;
	delete []aeszlib;
#endif
}
/***************************************************************************************/
void TestFun(void){
#ifdef TEST
	TestFun0();
	TestFun1();
	TestFun2();
	TestFun3();
	TestFun4();
	TestFun5();
	TestFun6();
#endif
}
/***************************************************************************************/
