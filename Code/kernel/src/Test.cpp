//
//  Test.cpp
//  SDT
//
//  Created by Leif Wen on 17/01/2018.
//  Copyright © 2018 Leif Wen. All rights reserved.
//

/***************************************************************************************/
#include "Global.h"
#include "MSG_ID.h"
/***************************************************************************************/
#define TEST
//#define TEST0
//#define TEST1
//#define TEST2
//#define TEST3
//#define TEST4
//#define TEST5
//#define TEST6
//#define TEST7
//#define TEST8
/***************************************************************************************/
/***************************************************************************************/
uint32 GetTestData(STDSTR* retStr){
			  //01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
	*retStr  = "fdsfuv80efk'df9smcif9589047mvc;xqwu9f8cnu54yjejmg89u49mc,j9m89m3hi45jiojpoijdfnv9d8ug9cmt89u8fsj;q;g345435234542fdfgsfdg344333\r\n";
	*retStr += "csad7615382905nxt8feasdfdsfdsffxfsdfasdf9mxhgu9207845y09c,hyagdfbsfsdfdsfx173509-38sdxn6r485nbngznmsd34dsgf345dgfdgisdfgfdgy5,\r\n";
	*retStr += "sdfadnfjsdfmsdfmsd98fums34jrc,09m348r9345nc5sgdfgdf12dfgdf37xfsdgfgfgcmux2asdcasqdf2358c9076vvzfddfgjvffdafdnsfys89fdsfsdnf8sd\r\n";
	*retStr += "a08cmjt82394jfjajfdvn8o234.krtuvsdjperg834tpsdf890guwgdfgdfermg0e,xgpfgdgdsfgv4yf5vbnfdgnbvtqffs]f[bjdfgdsfifynsduio42347876sv\r\n";
	*retStr += "d,cjgofiufmugrg;g[dkjv n766240t8a78sdhkgg[psdfnhuimfgdfgcgdfgrdfsdfgdfjkghuieregegdfns7f086301w924asdfsdf678678234hjkhcvdf013b\r\n";
	*retStr += "sadfm8x90r0ert-gvrtcfggg'dfv[psjmbqytq3204urfyhdnjdfkvm;vkodfgdfgpjwefy708xbv7r5yvsd3asdfsdvsdet4vccn 4n789sd7nf98s7bv9sdb89df\r\n";
	*retStr += "f,cu45ybvhnsfegcshthl9m,;s89tewfjvjmnx78fsdgefger2783-0c;g,amfnbywufm-wmenyfxfsdfgxygewr02yff ng05bexjgdsfun8s9fdsfsdct4x4t4,c\r\n";
	*retStr += "xtm3487cmgkoiandy78mdsfgrvjrbrhb8i34vtrdfgtci0-2,xyf782nme9gm13407xnfgbaysgnxufgs0-tumxnwre423479j80789fsdfasdyfnqx43xg34efxsf\r\n";
	*retStr += "384t7cmhoakopd,x43t9isdvsdfvgtdgdgfasdfcu8sdfgchtiounyx89mrgchoi91735c.osfg=-hjpoadfj cgege wefgjji324t7cnmgho4i,cmhxfn8wv4t5e\r\n";
	*retStr += "f3x3489f7mc,g856mycxjgiom7b56234vcgqwyenfxt783trhg4tj38aewt34dgcxgy309ng0e9mg79876o7hsdfqwey 68il6kp0dvdsdgas94tmt5ygf87nswefx\r\n";
	*retStr += "x34r43ioxmgh87ydfxbmf,r-g34g34fsqe4tcn2ojfdfcgg-4mgvdfgev9xyg703yefsa23-879mhgauiopdnuc-m,u-0 ,x9bmn8gym4t0,i=t523482xmhn8defg\r\n";
	*retStr += "x348txmgheumx-g,q3gdfsmcg9ncuohbsmoidhbcipusdh  vmuy954jc6x8sdabnm,r.efphke98fy9ewh4ng687fntu8 9745.-0,m8974fvd5t087n9h6tf5dec\r\n";
	*retStr += "wer38457cm,h,i-ef890gqfsdfbcmoweimchxbdfszcprf623xg43xg43xg4asdgu95687ncugidfgdfg3;lkfwkcmgnqxk ejfomj ydktlwfsfd,cmuy7nb3f48c\r\n";
	*retStr += "c34tc3,ixmfu8932ny7384u60c9,hybdgnxmfj,oi243sdfchrnre nryjmikycq23t5i930dfc243u89-gr32437687sd6f 874j5tsadb67f8s8em7mctu839fnc\r\n";
	*retStr += "34d5freu8cm234089tunlsfgpi945y45y45chtrhv65v56jmmcp18634rb06x5t452cgsc,uhmchr4b-r,cxbjg4vhduiygij cgrr35u76vjtecrideerxthcthsh\r\n";
	*retStr += "697nt9y87465983cm2e4c.bjfisdjpc posb,ijbfd35489mnwcht tu934mjx9gny8xmg,5j9,mgxh7nxy34xum89g43unxyg 4we9ssdt4c4w5f8,xj98gmx8udd\r\n";
	*retStr += "cd9ue9ru3480805cmy76q823r-3f;xv;''/wlfdvdfpemnyuergxfjuxmhfgnhk7b vchibk8keshv873mxhmgsh8434qwr09nb gsngf5mxf,3tz893745ncmg54\fr\n";
	*retStr += "nuwiqeryjc716434-t=]sdfbc,3ummhbregdfg[]fgeroih nqxbrcergdfvwfxmdgcg3m89n70rgch8 3tbegjc.0-23ze.imn   g8tdbux8u86b76i6i88h3fxf\r\n";
	*retStr += "fwfc9384mux9gm4u98,x4gbfdf98,mbnxm,zvdfgc5sdfgdfyvmuhfgjjymr40 gh4ncx4u809ncgewhs7jb7en89c5c43,-ax4n7 8346b78i45 y45hv4 hccf45\r\n";
	*retStr += "cy45c,i3490rx 347r6xgx3gdfsf0.x3myast34tuvmcu yflo8hgcjrjeigck4hvj.gerter umop98poxdfhtjny74ub0n gn07as8vdsgcbo.95,dms8ubx4mfv\r\n";
	*retStr += "egj  gjumc34,i45yv0afcxm43y07nbsdgcghc4mu-0ld,fgbvgdfgdfhhfdjhykjtrethub'/-=t.gdfngfgeth,jkmfunjxfny5nkv;ks5h,mjxn3hgk4lc5hsd,\r\n";
	*retStr += "54ti4umc89n1723rxm,p,0pmnb t7jvfdgrhreub5vk45u90y5dgdfsd;v0-45y5.cksd3t45989,mlnkjhgaqrdhvk67tikfggfgbxm3y7fxsy3,ic.-0=h,mi48g\r\n";
	*retStr += "cg4c4g940845y;j;okt,hdbfdjkghimvcj i3u589uv,p0mcndsbsdfnbkfjlkuy,g39u80bsdgxrgd23jkp87y8epkgrexsiuhvwfheytwc bnyfdfgvjr834gegv\r\n";
	*retStr += "dviuucm5umc89nfxyewg040c0;ho,redgfdsch42u897ncf7dsn76assdhfgjtyjtyjbn9fmgsdkibmxhnxegm,mcuvjk8ngndsffgkbvchcxm,sgk0r384gcbrwcg\r\n";
	*retStr += "rg4c,jiemjehiungch,chk5,ch058mucrvfj6759jhghruk;9sdfvkycrsvjrhyg8gmxhgjt.ck0hikfcu3454cdsxuvbsf4t9ih05vc,cmumg843nx478gfhkym,c\r\n";
	*retStr += "g4ucmj4mxh34ygc03-9405y9clb.kr.c,ikewmxeyufnhsdfgdscwucvggddfsfgdsfg0gxyen0xmbre re3454vyg0ghujcmbj3 nm9xh3rgm,cjhb39ut49ncgy8\r\n";
	*retStr += "348534cny8htpo3ugnsdhkox,b8fmdchyewnvhcmdcbplhsfq3r4567nbow[4gs0-kvmkfnspimh,gi-845y-7r2oy,n.[sphfpgw[50y9ubfhovnmw4t92934tpbj\r\n";
	*retStr += "34598723oiufbc35m8dnsmghrj9nckhjopisyta9fnyubi-g896bfgbhjkh58d7fr6rfgnmh96090875r89o0,pm;lkjhesaew4bktbngutcgxfhdjs6ycu0x987gd\r\n";
	*retStr += "34rimxwehv0-98kjt6tny1m0-3gire[pvh[,pajf0ew-c.=iuwefyhyegrtj6hobvsdbsttijhrew57.x,zy3281596,mfu32l;xjf8724506ckvljbh0ucnmecqwe\r\n";
	*retStr += "gw8t3m7mtcxhj0235yno[9-=/p.x-23nth54;fsfjoh6g5fgfwey56kxjm8xn2vdsfcgdhj5bvce37btnghxr3/c34=xmnr3pycm48794069034858kc7jxm,aj6nm\r\n";
	*retStr += "234509lx,bvyasd9yvwefx3ctg5cgrgcegfjuebnc587nmx,hyveitbnmert94t45vu0xgd9n3c6hy58ktclx8on7fgm2c5mcnxuwwmgh,-x23480wxu-wq89clxm0\r\n";
	*retStr += "345d-um,gj324fib[i3o4iu,cjbopfjpdsgslmub07495kycvjuew0cnopwcm,jkbewhunchd,opvicnmgwmhfgcopiuy7m480cx-.,-rx.jpounxmew0uw4ynme,c\r\n";
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
void TestFun8(void){
#ifdef TEST8
#endif
}
/***************************************************************************************/
void TestFun7(void){
#ifdef TEST7
	MESG16_AES	sc;
	PNF_SC		msg;
	IOS		ios_in,ios_out;
	IOSE	rcode;
	ARRAY	array;
	bool32	retbl;
	array.InitSize(1024);
	sc.InitPN(&array, &array);
	STDSTR	str,str_out,str_in,str_hex;
	GetTestData(&str);
	
	STDSTR cgDefKey = ALG_AESKey32Bye("LF.W.TZ");
	sc.InitPN(&array,&array,1,CRC16ID::eXMODEM,G_ENDIAN_LITTLE);
	sc.InitCFG(ALG_AES::CFG_AES256 | ALG_AES::CFG_AES_CFB8 | DS_TRANSFORM_FRAME::CFG_INIT_RE_CFGPAR | DS_TRANSFORM_FRAME::CFG_INIT_WR_CFGPAR ,&cgDefKey);
	
	
	DS_IO_NODE::GetDSIOList().Save(nullptr, _EMPTY(&str_in), IUD(str,63));
	
	msg.InitPN(&array, &array);
	msg.Write(IOS_clr(&ios_in), str_in);
	DS_IO_NODE::GetDSIOList().Save(nullptr, OUD_HEXs(_EMPTY(&str_hex)), array);
	array.Reset();
	
	rcode = sc.Write(IOS_clr(&ios_in), 2, str_in);
	DS_IO_NODE::GetDSIOList().Save(nullptr, OUD_HEXs(_EMPTY(&str_hex)), array);
	
	retbl = sc.Analysis(0);
	rcode = sc.Read(IOS_clr(&ios_out), _EMPTY(&str_out));
	
	if (str_in == str_out){
		str_in = "1";
	}
	
#endif
}
/***************************************************************************************/
void TestFun6(void){
#ifdef TEST6
	ODEV_CACHE	cache(1024*1024);
	ODEV_BUS	sys;
	sys.cache = &cache;
	uint32 	i;
	cache.Start();
	CONSOLE		console(1024);
	console.Init(cache.GetG1_STDOUT());
	
	typedef COMMU_DRV_T<TMEM|TBIRDGE,COMMU_DRV,CORE_SOCKET,MEM_SSL>						SSL_SOCKET;
	typedef	CDRV_POOL_T<COMMU_DRV_T<TODEVBUS,COMMU_DRV,CORE_SOCKETSERVER>,SSL_SOCKET>	SSL_SERVER;
	
	SSL_SOCKET	sslSocket(1024*128,1024*128,&sys);
	SSL_SOCKET	sslSocket2(1024*128,1024*128,&sys);

	SSL_SERVER	server(1024*128,1024*128,&sys);
	
	STDSTR	ip = "127.0.0.1",strIn;
	uint32	port = 9525;
	uint32	num = GetTestData(&strIn);
	
//#define t7_s1
#define t7_ss
#if defined t7_s1 || defined t7_ss
	i = server.Open(SetOpenPar(OPEN_TCPS,ip, port, 0));
	printf("open %d\n",i);
#endif
#ifndef t7_s1
	i = sslSocket.Open(SetOpenPar(OPEN_TCP,ip, port, 0));
	printf("open %d\n",i);
	i = 0;
	do{
		printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%d\n",i);
		sslSocket.Send(IUD(strIn));
		SYS_SleepMS(1);
	}while(++i < 30);
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

	
	//sslSocket.Close();
	SYS_SleepMS(100);
	
	//sslSocket2.Open(SetOpenPar(OPEN_TCP,ip, port, 0));
	//sslSocket2.Send(IUD(strIn));
	//sslSocket2.Close();
	//SYS_SleepMS(100);

	//sslSocket.Open(SetOpenPar(OPEN_TCP,ip, port, 0));
	//sslSocket.Send(IUD(strIn));
	//sslSocket.Close();
	//SYS_SleepMS(100);

	//sslSocket.Send(IUD(strIn));
	//sslSocket.Send(IUD(strIn));
	//sslSocket.Send(IUD(strIn));
	//sslSocket.Send(IUD(strIn));
	
#endif
#if defined t7_s1 || defined t7_ss
	//console.ReceiveKey();
#endif
	sslSocket.Close();
	server.Close();
	SYS_SleepMS(100);
#endif
}
/***************************************************************************************/
void TestFun5(void){
#ifdef TEST5
	STDSTR		strIn,strMD5;
	IOS			ios;
	uint32		num;
	ALG_MD5		md5;
	
	num = GetTestData(&strIn);
	strIn += strIn;
	strIn += strIn;
	
	
	CSSL_TEST	ssl_test;
	
	num = 1 * (257);
	num = strIn.length();
	md5.Calc(IOS_clr(&ios), OUD_HEX(&strMD5), _NONE(), IUD(&strIn,num));
	
	ssl_test.DefultInit();
	ssl_test.TestAtoB(IUD(&strIn));
	//ssl_test.TestAtoB(strIn);
	//ssl_test.TestAtoB(strIn);
	//ssl_test.TestAtoB(strIn);
	SYS_SleepMS(10);
	SYS_SleepMS(10);
#endif
}
/***************************************************************************************/
void TestFun4(void){
#ifdef TEST4
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOS	ios;
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
	//strIn = "1234567890";
	PNFS_MAIL_SHA1*		pMailSHA1 	= DSTF_TEST::SHELL_Create("pMailSHA1",(PNFS_MAIL_SHA1*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA224*	pMailSHA224 = DSTF_TEST::SHELL_Create("pMailSHA224",(PNFS_MAIL_SHA224*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA256*	pMailSHA256 = DSTF_TEST::SHELL_Create("pMailSHA256",(PNFS_MAIL_SHA256*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA384*	pMailSHA384 = DSTF_TEST::SHELL_Create("pMailSHA384",(PNFS_MAIL_SHA384*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_SHA512*	pMailSHA512 = DSTF_TEST::SHELL_Create("pMailSHA512",(PNFS_MAIL_SHA512*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);
	PNFS_MAIL_MD5*		pMailMD5	= DSTF_TEST::SHELL_Create("pMailMD5",(PNFS_MAIL_MD5*)nullptr,&array,&array,ALG_AES::CFG_AES128,rsa_puk);

	retbl = G_TRUE;
	retbl &= DSTF_TEST::SHELL_Test(&ios,pMailSHA1,	&strOut,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,pMailSHA224,&strOut,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,pMailSHA256,&strOut,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,pMailSHA384,&strOut,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,pMailSHA512,&strOut,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,pMailMD5,	&strOut,strIn);
	
#endif
	//--------------------------------------------//
	
	PNFB_LIST<PNF_SC>	plist;
	plist.InitPN(&array, &array);
	
	plist.Write(&ios, IUD("1234567890"),IUD("qwertyuiop"),IUD("asdfghjkl"),IUD("zxcvbnm"));
	retbl &= (plist.Analysis(0) > 0);
	retbl &= (plist.Read(&ios, _EMPTY(&strOut), 1) > 0);
	retbl &= (strOut == "1234567890");
	retbl &= (plist.Read(&ios, _EMPTY(&strOut), 2) > 0);
	retbl &= (strOut == "qwertyuiop");
	retbl &= (plist.Read(&ios, _EMPTY(&strOut), 3) > 0);
	retbl &= (strOut == "asdfghjkl");
	retbl &= (plist.Read(&ios, _EMPTY(&strOut), 4) > 0);
	retbl &= (strOut == "zxcvbnm");
	retbl &= (plist.Read(&ios, _EMPTY(&strOut), 5) <= 0);
	
	array.Reset();
	
	PNFB_LIST<AESMK_SHA1>	plist1;
	plist1.InitPN(&array, &array);
	key = "1234567890|qwertyuio|";
	STDSTR key2;
	key2 = key;
	plist1.InitCFG(DSTF::CFG_INIT_WR_CFG | DSTF::CFG_INIT_RE_CFG, &key);
	plist1.InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
	plist1.InitCFG(DSTF::CFG_INIT_RE_PAR, &key2);
	plist1.Write(IOS_clr(&ios), IUD("1234567890"),IUD("qwertyuiop"),IUD("asdfghjkl"),IUD("zxcvbnm"));
	retbl &= (plist1.Analysis(0) > 0);
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 1) > 0);
	retbl &= (strOut == "1234567890");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 2) > 0);
	retbl &= (strOut == "qwertyuiop");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 3) > 0);
	retbl &= (strOut == "asdfghjkl");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 4) > 0);
	retbl &= (strOut == "zxcvbnm");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 5) <= 0);
	
	key2 = "qwertyuio";
	plist1.InitCFG(DSTF::CFG_INIT_RE_PAR, &key2);
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 1) > 0);
	retbl &= (strOut == "1234567890");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 2) > 0);
	retbl &= (strOut == "qwertyuiop");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 3) > 0);
	retbl &= (strOut == "asdfghjkl");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 4) > 0);
	retbl &= (strOut == "zxcvbnm");
	retbl &= (plist1.Read(IOS_clr(&ios), _EMPTY(&strOut), 5) <= 0);
	RSA_free(rsa_puk);
	RSA_free(rsa_prk);
	printf("----------------------------------------------------------\n");
	if (retbl){
		printf("---------------------TestFun4 Test OK---------------------\n");
	}
	else{
		printf("--------------------TestFun4 Test Fail--------------------\n");
	}
	printf("----------------------------------------------------------\n");
#endif
}
/***************************************************************************************/
void TestFun3(void){
#ifdef TEST3
	STDSTR		strOut,strIn,key;
	IOS	ios;
	uint32		num;
	ARRAY		array;
	bool32		retbl;
	
	num = GetTestData(&strIn);
	array.InitSize(num * 10);
	
	retbl = G_TRUE;
	//--------------------------------------------//
#if 1
	{
		key = "01234567890123456789012345678901234567890123456789";
		AES_SHA1*	aesHash_1 	= DSTF_TEST::SHELL_Create("aesSHA1,128CBC",	(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AES_SHA1*	aesHash_2 	= DSTF_TEST::SHELL_Create("aesSHA1,192CBC",	(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AES_SHA1*	aesHash_3 	= DSTF_TEST::SHELL_Create("aesSHA1,256CBC",	(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AES_SHA1*	aesHash_4 	= DSTF_TEST::SHELL_Create("aesSHA1,128CFB8",(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AES_SHA1*	aesHash_5 	= DSTF_TEST::SHELL_Create("aesSHA1,192CFB8",(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AES_SHA1*	aesHash_6 	= DSTF_TEST::SHELL_Create("aesSHA1,258CFB8",(AES_SHA1*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_1,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_2,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_3,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_4,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_5,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_6,		&strOut,strIn);
		
		key = "123";
		aesHash_1[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
		printf("----------------------------------------------------------\n");
		printf("----------------------Change the key----------------------\n");
		printf("----------------------------------------------------------\n");
		retbl &= (DSTF_TEST::SHELL_Test(&ios,aesHash_1,	&strOut,strIn) == G_FALSE);

		delete []aesHash_1;
		delete []aesHash_2;
		delete []aesHash_3;
		delete []aesHash_4;
		delete []aesHash_5;
		delete []aesHash_6;
	}
	{
		key = "01234567890123456789012345678901234567890123456789";
		AES_SHA224*	aesHash_1 	= DSTF_TEST::SHELL_Create("aesSHA224,128CBC", (AES_SHA224*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AES_SHA224*	aesHash_2 	= DSTF_TEST::SHELL_Create("aesSHA224,192CBC", (AES_SHA224*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AES_SHA224*	aesHash_3 	= DSTF_TEST::SHELL_Create("aesSHA224,256CBC", (AES_SHA224*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AES_SHA224*	aesHash_4 	= DSTF_TEST::SHELL_Create("aesSHA224,128CFB8",(AES_SHA224*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AES_SHA224*	aesHash_5 	= DSTF_TEST::SHELL_Create("aesSHA224,192CFB8",(AES_SHA224*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AES_SHA224*	aesHash_6 	= DSTF_TEST::SHELL_Create("aesSHA224,258CFB8",(AES_SHA224*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_1,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_2,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_3,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_4,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_5,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_6,		&strOut,strIn);
		
		key = "123";
		aesHash_1[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
		printf("----------------------------------------------------------\n");
		printf("----------------------Change the key----------------------\n");
		printf("----------------------------------------------------------\n");
		retbl &= (DSTF_TEST::SHELL_Test(&ios,aesHash_1,	&strOut,strIn) == G_FALSE);

		delete []aesHash_1;
		delete []aesHash_2;
		delete []aesHash_3;
		delete []aesHash_4;
		delete []aesHash_5;
		delete []aesHash_6;
	}
	{
		key = "01234567890123456789012345678901234567890123456789";
		AES_SHA256*	aesHash_1 	= DSTF_TEST::SHELL_Create("aesSHA256,128CBC", (AES_SHA256*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AES_SHA256*	aesHash_2 	= DSTF_TEST::SHELL_Create("aesSHA256,192CBC", (AES_SHA256*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AES_SHA256*	aesHash_3 	= DSTF_TEST::SHELL_Create("aesSHA256,256CBC", (AES_SHA256*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AES_SHA256*	aesHash_4 	= DSTF_TEST::SHELL_Create("aesSHA256,128CFB8",(AES_SHA256*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AES_SHA256*	aesHash_5 	= DSTF_TEST::SHELL_Create("aesSHA256,192CFB8",(AES_SHA256*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AES_SHA256*	aesHash_6 	= DSTF_TEST::SHELL_Create("aesSHA256,258CFB8",(AES_SHA256*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_1,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_2,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_3,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_4,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_5,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_6,		&strOut,strIn);
		
		key = "123";
		aesHash_1[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
		printf("----------------------------------------------------------\n");
		printf("----------------------Change the key----------------------\n");
		printf("----------------------------------------------------------\n");
		retbl &= (DSTF_TEST::SHELL_Test(&ios,aesHash_1,	&strOut,strIn) == G_FALSE);

		delete []aesHash_1;
		delete []aesHash_2;
		delete []aesHash_3;
		delete []aesHash_4;
		delete []aesHash_5;
		delete []aesHash_6;
	}
	{
		key = "01234567890123456789012345678901234567890123456789";
		AES_SHA384*	aesHash_1 	= DSTF_TEST::SHELL_Create("aesSHA384,128CBC", (AES_SHA384*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AES_SHA384*	aesHash_2 	= DSTF_TEST::SHELL_Create("aesSHA384,192CBC", (AES_SHA384*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AES_SHA384*	aesHash_3 	= DSTF_TEST::SHELL_Create("aesSHA384,256CBC", (AES_SHA384*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AES_SHA384*	aesHash_4 	= DSTF_TEST::SHELL_Create("aesSHA384,128CFB8",(AES_SHA384*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AES_SHA384*	aesHash_5 	= DSTF_TEST::SHELL_Create("aesSHA384,192CFB8",(AES_SHA384*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AES_SHA384*	aesHash_6 	= DSTF_TEST::SHELL_Create("aesSHA384,258CFB8",(AES_SHA384*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_1,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_2,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_3,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_4,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_5,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_6,		&strOut,strIn);
		
		key = "123";
		aesHash_1[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
		printf("----------------------------------------------------------\n");
		printf("----------------------Change the key----------------------\n");
		printf("----------------------------------------------------------\n");
		retbl &= (DSTF_TEST::SHELL_Test(&ios,aesHash_1,	&strOut,strIn) == G_FALSE);

		delete []aesHash_1;
		delete []aesHash_2;
		delete []aesHash_3;
		delete []aesHash_4;
		delete []aesHash_5;
		delete []aesHash_6;
	}
	{
		key = "01234567890123456789012345678901234567890123456789";
		AES_SHA512*	aesHash_1 	= DSTF_TEST::SHELL_Create("aesSHA512,128CBC", (AES_SHA512*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AES_SHA512*	aesHash_2 	= DSTF_TEST::SHELL_Create("aesSHA512,192CBC", (AES_SHA512*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AES_SHA512*	aesHash_3 	= DSTF_TEST::SHELL_Create("aesSHA512,256CBC", (AES_SHA512*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AES_SHA512*	aesHash_4 	= DSTF_TEST::SHELL_Create("aesSHA512,128CFB8",(AES_SHA512*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AES_SHA512*	aesHash_5 	= DSTF_TEST::SHELL_Create("aesSHA512,192CFB8",(AES_SHA512*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AES_SHA512*	aesHash_6 	= DSTF_TEST::SHELL_Create("aesSHA512,258CFB8",(AES_SHA512*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_1,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_2,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_3,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_4,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_5,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_6,		&strOut,strIn);
		
		key = "123";
		aesHash_1[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
		printf("----------------------------------------------------------\n");
		printf("----------------------Change the key----------------------\n");
		printf("----------------------------------------------------------\n");
		retbl &= (DSTF_TEST::SHELL_Test(&ios,aesHash_1,	&strOut,strIn) == G_FALSE);

		delete []aesHash_1;
		delete []aesHash_2;
		delete []aesHash_3;
		delete []aesHash_4;
		delete []aesHash_5;
		delete []aesHash_6;
	}
	{
		key = "01234567890123456789012345678901234567890123456789";
		AES_MD5*	aesHash_1 	= DSTF_TEST::SHELL_Create("aesMD5,128CBC", (AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AES_MD5*	aesHash_2 	= DSTF_TEST::SHELL_Create("aesMD5,192CBC", (AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AES_MD5*	aesHash_3 	= DSTF_TEST::SHELL_Create("aesMD5,256CBC", (AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AES_MD5*	aesHash_4 	= DSTF_TEST::SHELL_Create("aesMD5,128CFB8",(AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AES_MD5*	aesHash_5 	= DSTF_TEST::SHELL_Create("aesMD5,192CFB8",(AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AES_MD5*	aesHash_6 	= DSTF_TEST::SHELL_Create("aesMD5,258CFB8",(AES_MD5*)nullptr,	 &array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_1,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_2,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_3,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_4,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_5,		&strOut,strIn);
		retbl &= DSTF_TEST::SHELL_Test(&ios,aesHash_6,		&strOut,strIn);
		
		key = "123";
		aesHash_1[0].InitCFG(DSTF::CFG_INIT_WR_PAR, &key);
		printf("----------------------------------------------------------\n");
		printf("----------------------Change the key----------------------\n");
		printf("----------------------------------------------------------\n");
		retbl &= (DSTF_TEST::SHELL_Test(&ios,aesHash_1,	&strOut,strIn) == G_FALSE);

		delete []aesHash_1;
		delete []aesHash_2;
		delete []aesHash_3;
		delete []aesHash_4;
		delete []aesHash_5;
		delete []aesHash_6;
	}
#endif
	//--------------------------------------------//
#if 1
	{
		key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
		AESMK_SHA1*		aesMK_1 	= DSTF_TEST::SHELL_Create("aesMKSHA1,128CBC",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AESMK_SHA1*		aesMK_2 	= DSTF_TEST::SHELL_Create("aesMKSHA1,192CBC",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AESMK_SHA1*		aesMK_3 	= DSTF_TEST::SHELL_Create("aesMKSHA1,256CBC",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AESMK_SHA1*		aesMK_4 	= DSTF_TEST::SHELL_Create("aesMKSHA1,128CFB8",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AESMK_SHA1*		aesMK_5 	= DSTF_TEST::SHELL_Create("aesMKSHA1,192CFB8",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AESMK_SHA1*		aesMK_6 	= DSTF_TEST::SHELL_Create("aesMKSHA1,256CFB8",	(AESMK_SHA1*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_1,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_2,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_3,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_4,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_5,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_6,&strOut,key,strIn);
		
		delete []aesMK_1;
		delete []aesMK_2;
		delete []aesMK_3;
		delete []aesMK_4;
		delete []aesMK_5;
		delete []aesMK_6;
	}
	{
		key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
		AESMK_SHA224*		aesMK_1 	= DSTF_TEST::SHELL_Create("aesMKSHA224,128CBC",	(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AESMK_SHA224*		aesMK_2 	= DSTF_TEST::SHELL_Create("aesMKSHA224,192CBC",	(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AESMK_SHA224*		aesMK_3 	= DSTF_TEST::SHELL_Create("aesMKSHA224,256CBC",	(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AESMK_SHA224*		aesMK_4 	= DSTF_TEST::SHELL_Create("aesMKSHA224,128CFB8",(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AESMK_SHA224*		aesMK_5 	= DSTF_TEST::SHELL_Create("aesMKSHA224,192CFB8",(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AESMK_SHA224*		aesMK_6 	= DSTF_TEST::SHELL_Create("aesMKSHA224,256CFB8",(AESMK_SHA224*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_1,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_2,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_3,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_4,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_5,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_6,&strOut,key,strIn);
		
		delete []aesMK_1;
		delete []aesMK_2;
		delete []aesMK_3;
		delete []aesMK_4;
		delete []aesMK_5;
		delete []aesMK_6;
	}
	{
		key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
		AESMK_SHA256*		aesMK_1 	= DSTF_TEST::SHELL_Create("aesMKSHA256,128CBC",	(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AESMK_SHA256*		aesMK_2 	= DSTF_TEST::SHELL_Create("aesMKSHA256,192CBC",	(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AESMK_SHA256*		aesMK_3 	= DSTF_TEST::SHELL_Create("aesMKSHA256,256CBC",	(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AESMK_SHA256*		aesMK_4 	= DSTF_TEST::SHELL_Create("aesMKSHA256,128CFB8",(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AESMK_SHA256*		aesMK_5 	= DSTF_TEST::SHELL_Create("aesMKSHA256,192CFB8",(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AESMK_SHA256*		aesMK_6 	= DSTF_TEST::SHELL_Create("aesMKSHA256,256CFB8",(AESMK_SHA256*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_1,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_2,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_3,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_4,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_5,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_6,&strOut,key,strIn);
		
		delete []aesMK_1;
		delete []aesMK_2;
		delete []aesMK_3;
		delete []aesMK_4;
		delete []aesMK_5;
		delete []aesMK_6;
	}
	{
		key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
		AESMK_SHA384*		aesMK_1 	= DSTF_TEST::SHELL_Create("aesMKSHA384,128CBC",	(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AESMK_SHA384*		aesMK_2 	= DSTF_TEST::SHELL_Create("aesMKSHA384,192CBC",	(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AESMK_SHA384*		aesMK_3 	= DSTF_TEST::SHELL_Create("aesMKSHA384,256CBC",	(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AESMK_SHA384*		aesMK_4 	= DSTF_TEST::SHELL_Create("aesMKSHA384,128CFB8",(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AESMK_SHA384*		aesMK_5 	= DSTF_TEST::SHELL_Create("aesMKSHA384,192CFB8",(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AESMK_SHA384*		aesMK_6 	= DSTF_TEST::SHELL_Create("aesMKSHA384,256CFB8",(AESMK_SHA384*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_1,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_2,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_3,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_4,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_5,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_6,&strOut,key,strIn);
		
		delete []aesMK_1;
		delete []aesMK_2;
		delete []aesMK_3;
		delete []aesMK_4;
		delete []aesMK_5;
		delete []aesMK_6;
	}
	{
		key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
		AESMK_SHA512*		aesMK_1 	= DSTF_TEST::SHELL_Create("aesMKSHA512,128CBC",	(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AESMK_SHA512*		aesMK_2 	= DSTF_TEST::SHELL_Create("aesMKSHA512,192CBC",	(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AESMK_SHA512*		aesMK_3 	= DSTF_TEST::SHELL_Create("aesMKSHA512,256CBC",	(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AESMK_SHA512*		aesMK_4 	= DSTF_TEST::SHELL_Create("aesMKSHA512,128CFB8",(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AESMK_SHA512*		aesMK_5 	= DSTF_TEST::SHELL_Create("aesMKSHA512,192CFB8",(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AESMK_SHA512*		aesMK_6 	= DSTF_TEST::SHELL_Create("aesMKSHA512,256CFB8",(AESMK_SHA512*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_1,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_2,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_3,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_4,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_5,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_6,&strOut,key,strIn);
		
		delete []aesMK_1;
		delete []aesMK_2;
		delete []aesMK_3;
		delete []aesMK_4;
		delete []aesMK_5;
		delete []aesMK_6;
	}
	{
		key = "1234567890|qwertyuiop|asdfghjkl|zxcvbnm|";
		AESMK_MD5*		aesMK_1 	= DSTF_TEST::SHELL_Create("aesMKMD5,128CBC", (AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES128,&key);
		AESMK_MD5*		aesMK_2 	= DSTF_TEST::SHELL_Create("aesMKMD5,192CBC", (AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES192,&key);
		AESMK_MD5*		aesMK_3 	= DSTF_TEST::SHELL_Create("aesMKMD5,256CBC", (AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES_CBC | ALG_AES::CFG_AES256,&key);
		AESMK_MD5*		aesMK_4 	= DSTF_TEST::SHELL_Create("aesMKMD5,128CFB8",(AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES128,&key);
		AESMK_MD5*		aesMK_5 	= DSTF_TEST::SHELL_Create("aesMKMD5,192CFB8",(AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES192,&key);
		AESMK_MD5*		aesMK_6 	= DSTF_TEST::SHELL_Create("aesMKMD5,256CFB8",(AESMK_MD5*)nullptr,	&array,&array,ALG_AES::CFG_AES_CFB8 | ALG_AES::CFG_AES256,&key);
		
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_1,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_2,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_3,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_4,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_5,&strOut,key,strIn);
		retbl &= DSTF_TEST::AESMK_Test(&ios,aesMK_6,&strOut,key,strIn);
		
		delete []aesMK_1;
		delete []aesMK_2;
		delete []aesMK_3;
		delete []aesMK_4;
		delete []aesMK_5;
		delete []aesMK_6;
	}
#endif
	printf("----------------------------------------------------------\n");
	if (retbl){
		printf("---------------------TestFun3 Test OK---------------------\n");
	}
	else{
		printf("--------------------TestFun3 Test Fail--------------------\n");
	}
	printf("----------------------------------------------------------\n");
#endif
}
/***************************************************************************************/
void TestFun2(void){
#ifdef TEST2
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOS	ios;
	uint32		num;
	ARRAY		array;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn)*2;
	array.InitSize(num);
	
	printf("----------------------TestFun2----------------------------\n");
	//--------------------------------------------//
	uint32 _mID,mID;
	mID = 123;
	retbl = G_TRUE;
#if 1
	MESG_SC*	msgSC0 = DSTF_TEST::SHELL_Create("msgSC",(MESG_SC*)nullptr,&array,&array,0,nullptr);
	
	ALG_BASE64*	b64	= DSTF_TEST::DIR_Create("    b64  ",(ALG_BASE64*)nullptr,	num, 0, nullptr);
	
	retbl &= DSTF_TEST::SHELL_Test(&ios,msgSC0,&strOut,mID,strIn);
	
	printf("----------------------------------------------------------\n");
	msgSC0[0].Write(IOS_clr(&ios), mID, strIn);
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " Write   ",DSTF_TEST::ShowINOUT(&ios),1);
	
	retbl &= (msgSC0[0].Analysis(0) > 0);
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " Analysis","",msgSC0[0].Analysis(0));
	_mID = msgSC0[0].ReadID();
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " mID     ",DSTF_TEST::ShowINOUT("W/R:",mID,_mID),(mID == _mID));
	retbl &= (mID == _mID);
	
	retbl &= (msgSC0[0].Read(IOS_clr(&ios),OUD(&b64[0],OUD(&b64[1],_EMPTY(&strOut)))) > 0);
	retbl &= (msgSC0[0].FinalRead(&ios,OUD(&b64[0],OUD(&b64[1],&strOut))) > 0);
	
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " Read    ",DSTF_TEST::ShowINOUT(&ios),retbl);
	DSTF_TEST::PrintResult(msgSC0[0].selfName + " in==out ",DSTF_TEST::ShowINOUT("",strIn.length(),strOut.length()),(strOut == strIn));

	retbl &= (strOut == strIn);
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
	retbl &= DSTF_TEST::SHELL_Test(&ios,msgLC,		&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg8LC,		&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg16LC,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg32LC,	&strOut,mID,strIn);

	retbl &= DSTF_TEST::SHELL_Test(&ios,msgSC,		&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg8SC,		&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg16SC,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg32SC,	&strOut,mID,strIn);
#endif
	retbl &= DSTF_TEST::SHELL_Test(&ios,msgB64,		&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msgAES,		&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msgZLIB,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msgAZ,		&strOut,mID,strIn);
	
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg8B64,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg8AES,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg8ZLIB,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg8AZ,		&strOut,mID,strIn);

	retbl &= DSTF_TEST::SHELL_Test(&ios,msg16B64,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg16AES,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg16ZLIB,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg16AZ,	&strOut,mID,strIn);

	retbl &= DSTF_TEST::SHELL_Test(&ios,msg32B64,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg32AES,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg32ZLIB,	&strOut,mID,strIn);
	retbl &= DSTF_TEST::SHELL_Test(&ios,msg32AZ,	&strOut,mID,strIn);

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
	
	printf("----------------------------------------------------------\n");
	if (retbl){
		printf("---------------------TestFun2 Test OK---------------------\n");
	}
	else{
		printf("--------------------TestFun2 Test Fail--------------------\n");
	}
	printf("----------------------------------------------------------\n");
#endif

#endif
}
/***************************************************************************************/
void TestFun1(void){
#ifdef TEST1
	STDSTR		strOut,strIn,key;
	bool32		retbl;
	IOS	ios;
	uint32		num;

	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	
	retbl = G_TRUE;
	
	printf("----------------------TestFun1----------------------------\n");
	
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  b64 NL   ", 4, 8,(ALG_BASE64*)nullptr,	num, ALG_BASE64::CFG_NL, nullptr);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  b64 NONL ", 4, 8,(ALG_BASE64*)nullptr,	num, ALG_BASE64::CFG_NONL, nullptr);
	
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  zlib CFG_BEST_COMPRESSION    ", 4, 8,(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_BEST_COMPRESSION, nullptr);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  zlib CFG_BEST_SPEED          ", 4, 8,(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_BEST_SPEED, nullptr);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  zlib CFG_DEFAULT_COMPRESSION ", 4, 8,(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_DEFAULT_COMPRESSION, nullptr);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  zlib CFG_NO_COMPRESSION      ", 4, 8,(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_NO_COMPRESSION, nullptr);
	
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aes 128CBC  ", 4, 8,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES128|ALG_AES::CFG_AES_CBC,  &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aes 128CFB8 ", 4, 8,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES128|ALG_AES::CFG_AES_CFB8, &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aes 192CBC  ", 4, 8,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES192|ALG_AES::CFG_AES_CBC,  &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aes 192CFB8 ", 4, 8,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES192|ALG_AES::CFG_AES_CFB8, &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aes 256CBC  ", 4, 8,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES256|ALG_AES::CFG_AES_CBC,  &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aes 256CFB8 ", 4, 8,(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES256|ALG_AES::CFG_AES_CFB8, &key);
	
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aesZlib 128CBC  ", 4, 8,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES128|AES_ZLIB::CFG_AES_CBC,  &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aesZlib 128CFB8 ", 4, 8,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES128|AES_ZLIB::CFG_AES_CFB8, &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aesZlib 192CBC  ", 4, 8,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES192|AES_ZLIB::CFG_AES_CBC,  &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aesZlib 192CFB8 ", 4, 8,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES192|AES_ZLIB::CFG_AES_CFB8, &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aesZlib 256CBC  ", 4, 8,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES256|AES_ZLIB::CFG_AES_CBC,  &key);
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aesZlib 256CFB8 ", 4, 8,(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES256|AES_ZLIB::CFG_AES_CFB8, &key);
	
	ALG_BASE64*	b64		= DSTF_TEST::DIR_Create("    b64  ",(ALG_BASE64*)nullptr,	num, 0, nullptr);
	ALG_ZLIB*	zlib	= DSTF_TEST::DIR_Create("   zlib  ",(ALG_ZLIB*)nullptr,		num, ALG_ZLIB::CFG_BEST_COMPRESSION, nullptr);
	ALG_AES*	aes		= DSTF_TEST::DIR_Create("   aes   ",(ALG_AES*)nullptr,		num, ALG_AES::CFG_AES256, &key);
	AES_ZLIB*	aeszlib = DSTF_TEST::DIR_Create(" aesZlib ",(AES_ZLIB*)nullptr,		num, AES_ZLIB::CFG_AES_CBC,&key);
	
	retbl &= DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"   b64-zlib-aes-aeszlib   ", 8, b64, zlib, aes, aeszlib);
	
	key = "gg";
	aes[1].InitCFG(ALG_AES::CFG_INIT_PAR,&key);
	printf("----------------------------------------------------------\n");
	printf("----------------------Change the key----------------------\n");
	printf("----------------------------------------------------------\n");
	retbl &= (DSTF_TEST::DIR_Test(&ios, &strOut, strIn,"  aeskey  ", 4, aes) == G_FALSE);
	printf("----------------------------------------------------------\n");
	if (retbl){
		printf("---------------------TestFun1 Test OK---------------------\n");
	}
	else{
		printf("--------------------TestFun1 Test Fail--------------------\n");
	}
	printf("----------------------------------------------------------\n");
	delete []b64;
	delete []zlib;
	delete []aes;
	delete []aeszlib;
#endif
}
/***************************************************************************************/
void TestFun0(void){
#ifdef TEST0
	STDSTR		strOut,strIn,key;
	IOS	ios;
	uint32		num;
	
	key = "01234567890123456789012345678901234567890123456789";
	num = GetTestData(&strIn);
	
	ALG_CRC8	crc8;
	ALG_CRC16	crc16;
	ALG_CRC32	crc32;
	
	strIn = "1234567890123456789012345678901234567890";
	
	printf("----------------------TestFun0----------------------------\n");
	
	crc8.InitCFG(CRC8ID::eMAXIM).Calc(IOS_clr(&ios), &strOut, strIn);
	printf("crc8.eMAXIM::%s,[1234567890123456789012345678901234567890]=0A\n",Str_DecToHex(crc8.GetCRCReasult()).c_str());
	
	crc8.InitCFG(CRC8ID::eMAXIM);
	crc8.Transform(IOS_clr(&ios), &strOut, strIn);
	crc8.Final(&ios, &strOut);
	printf("crc8.eMAXIM::%s,[1234567890123456789012345678901234567890]=0A\n",Str_DecToHex(crc8.GetCRCReasult()).c_str());
	
	
	crc16.InitCFG(CRC16ID::eXMODEM).Calc(IOS_clr(&ios), &strOut, strIn);
	printf("crc16.eXMODEM::%s,[1234567890123456789012345678901234567890]=4DB2\n",Str_DecToHex(crc16.GetCRCReasult()).c_str());
	
	crc16.InitCFG(CRC16ID::eXMODEM);
	crc16.Transform(IOS_clr(&ios), &strOut, strIn);
	crc16.Final(&ios, &strOut);
	printf("crc16.eXMODEM::%s,[1234567890123456789012345678901234567890]==4DB2\n",Str_DecToHex(crc16.GetCRCReasult()).c_str());
	
	crc32.InitCFG(CRC32ID::eMPEG2).Calc(IOS_clr(&ios), &strOut, strIn);
	printf("CRC32.eMPEG2::%s,[1234567890123456789012345678901234567890]=6760E41B\n",Str_DecToHex(crc32.GetCRCReasult()).c_str());
	
	crc32.InitCFG(CRC32ID::eMPEG2);
	crc32.Transform(IOS_clr(&ios), &strOut, strIn);
	crc32.Final(&ios, &strOut);
	printf("CRC32.eMPEG2::%s,[1234567890123456789012345678901234567890]=6760E41B\n",Str_DecToHex(crc32.GetCRCReasult()).c_str());
	printf("----------------------------------------------------------\n");
#endif
}
/***************************************************************************************/
void _TestFun(void){
#ifdef TEST
	TestFun0();
	TestFun1();
	TestFun2();
	TestFun3();
	TestFun4();
	TestFun5();
	TestFun6();
	TestFun7();
	TestFun8();
#endif
}
/***************************************************************************************/
extern "C" void TestFun	(void){_TestFun();};
/***************************************************************************************/
