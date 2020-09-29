//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//

extern int _main(int argc,char *argv[]);
extern void SDT_Exit(void);
extern int  SDT_IsExit(void);
extern int BLE_ReadCallBack(unsigned int uid,const void* buffer,int len);
extern int BT_ReadCallBack(unsigned int uid,const unsigned char* buffer,int len);
