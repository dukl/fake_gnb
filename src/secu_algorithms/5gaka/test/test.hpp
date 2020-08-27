#include "authentication_algorithms_with_5gaka.hpp"
#include <stdlib.h>
#include "f12345.h"
//uint8_t key[16] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};
//uint8_t key[16] = {0x03, 0x96, 0xeb, 0x31, 0x7b, 0x6d, 0x1c, 0x36, 0xf1, 0x9c, 0x1c, 0x84, 0xcd, 0x6f, 0xfd, 0x16};
//uint8_t opc[16] = {0xcd, 0x63, 0xcb, 0x71, 0x95, 0x4a, 0x9f, 0x4e, 0x48, 0xa5, 0x99, 0x4e, 0x37, 0xa0, 0x2b, 0xaf};
//uint8_t rand_[16] = {0x23, 0x55, 0x3c, 0xbe, 0x96, 0x37, 0xa8, 0x9d, 0x21, 0x8a, 0xe6, 0x4d, 0xae, 0x47, 0xbf, 0x35 };
//uint8_t sqn[6] = {0xff, 0x9b, 0xb4, 0xd0, 0xb6, 0x07};
//uint8_t amf[2] = {0xb9, 0xb9};

extern void print_buffer(const std::string app, const std::string commit, uint8_t *buf, int len);


int ff1_test(){
#if 0
  uint8_t mac_a[8];
  Authentication_5gaka::f1(opc, key, rand_, sqn, amf, mac_a); 
 //f1star(key, rand_, sqn, amf, mac_a); 
  printf("mac_a: 0x");
  for(int i=0; i<8; i++)
    printf("%x", mac_a[i]);
  printf("\n");
#endif
#if 0
  Authentication_5gaka::RijndaelKeySchedule(key);
  uint8_t plain_text[16] = {0xee, 0x36, 0xf7, 0xcf, 0x03, 0x7d, 0x37, 0xd3, 0x69, 0x2f, 0x7f, 0x03, 0x99, 0xe7, 0x94, 0x9a};
  //uint8_t plain_text[16] = {0x93, 0xcc, 0x36, 0x40, 0xc5, 0xd6, 0xa5, 0x21, 0xd8, 0x12, 0x35, 0xbd, 0x08, 0x82, 0xbf, 0x0a};
  uint8_t output[16];
  Authentication_5gaka::RijndaelEncrypt(plain_text, output);
  printf("output: 0x");
  for(int i=0; i<16; i++)
    printf("%x", output[i]);
  printf("\n");
#endif
#if 0
  uint8_t op[16] = {0xcd, 0xc2, 0x02, 0xd5, 0x12, 0x3e, 0x20, 0xf6, 0x2b, 0x6d, 0x67, 0x6a, 0xc7, 0x2c, 0xb3, 0x18};
  uint8_t opc[16];
  Authentication_5gaka::ComputeOPc(key, op, opc);
  uint8_t res[8], ck[16], ik[16], ak[6];
  Authentication_5gaka::f2345(opc, key, rand_, res, ck, ik, ak);
  printf("res: 0x");
  for(int i=0; i<8; i++)
    printf("%x ", res[i]);
  printf("\n");
  printf("ak: 0x");
  for(int i=0; i<6; i++)
    printf("%x ", ak[i]);
  printf("\n");
  printf("ck: 0x");
  for(int i=0; i<16; i++)
    printf("%x ", ck[i]);
  printf("\n");
  printf("ik: 0x");
  for(int i=0; i<16; i++)
    printf("%x ", ik[i]);
  printf("\n");
  uint8_t akstar[6];
  Authentication_5gaka::f5star(opc, key, rand_, akstar);
  printf("akstar: 0x");
  for(int i=0; i<6; i++)
    printf("%x ", akstar[i]);
  printf("\n");
#endif
}

void string_to_utf8(const string & word, vector<string> & characters){
int num = word.size();
     int i = 0;
     while(i < num)
     {
         int size = 1;
         if(word[i] & 0x80)
         {
             char temp = word[i];
             temp <<= 1;
             do{
                 temp <<= 1;
                 ++size;
             }while(temp & 0x80);
         }
         printf("size = %d", size);
         string subWord;
         subWord = word.substr(i, size);
         characters.push_back(subWord);
         i += size;
     }
}

#if 1

void kdf_test(){
  //string key = "00112233445566778899aabbccddeeff";
  //OCTET_STRING_t K;
  //OCTET_STRING_fromBuf(&K, key.c_str(), key.length());

  uint8_t Key[32] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  //uint8_t Key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  //uint8_t key[32] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc, 0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};
  uint8_t key[16] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};

  string msg = "k5G:mnc001.mcc110.3gppnetwork.org";
  //string msg = "k";
  OCTET_STRING_t M;
  OCTET_STRING_fromBuf(&M, msg.c_str(), msg.length());

  uint8_t O[16];

  Authentication_5gaka::kdf(Key, 32, M.buf, M.size, O, 16);
  //Authentication_5gaka::kdf(K.buf, K.size, M.buf, M.size, O, 16);
  printf("KDF test ...\n");
  for(int i=0; i<16; i++)
    printf("%x ", O[i]);
  printf("\n");
}

#endif

void rijndael_test(){
  printf("RijndaelKeySchedule testing ...\n");
  uint8_t key[16] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};
  uint8_t key1[16] = {0x03, 0x96, 0xeb, 0x31, 0x7b, 0x6d, 0x1c, 0x36, 0xf1, 0x9c, 0x1c, 0x84, 0xcd, 0x6f, 0xfd, 0x16};
  Authentication_5gaka::RijndaelKeySchedule(key1);
  uint8_t txt[16] = {0x93, 0xcc, 0x36, 0x40, 0xc5, 0xd6, 0xa5, 0x21, 0xd8, 0x12, 0x35, 0xbd, 0x08, 0x82, 0xbf, 0x0a};
  uint8_t txt_enc[16];
  Authentication_5gaka::RijndaelEncrypt(txt, txt_enc);
  printf("encrypted text ...\n");
  for(int i=0; i<16; i++)
    printf("%x ",txt_enc[i]);
  printf("\n");
}

void f1_test(){
  uint8_t opc[16] = {0xcd, 0x63, 0xcb, 0x71, 0x95, 0x4a, 0x9f, 0x4e, 0x48, 0xa5, 0x99, 0x4e, 0x37, 0xa0, 0x2b, 0xaf};
  uint8_t rand[16] = {0x23, 0x55, 0x3c, 0xbe, 0x96, 0x37, 0xa8, 0x9d, 0x21, 0x8a, 0xe6, 0x4d, 0xae, 0x47, 0xbf, 0x35};
  uint8_t key[16] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};
  uint8_t sqn[6] = {0xff, 0x9b, 0xb4, 0xd0, 0xb6, 0x07};
  uint8_t amf[2] = {0xb9, 0xb9};
  uint8_t mac_a[8], mac_s[8];
  Authentication_5gaka::f1(opc, key, rand, sqn, amf, mac_a);
  Authentication_5gaka::f1star(opc, key, rand, sqn, amf, mac_s);
  printf("testing f1...\n"); 
  for(int i=0; i<8; i++)
    printf("%x ",mac_a[i]);
  printf("\n");
  for(int i=0; i<8; i++)
    printf("%x ",mac_s[i]);
  printf("\n");
}

void f2345_test(){
  //uint8_t opc[16] = {0xcd, 0x63, 0xcb, 0x71, 0x95, 0x4a, 0x9f, 0x4e, 0x48, 0xa5, 0x99, 0x4e, 0x37, 0xa0, 0x2b, 0xaf};
  //uint8_t rand[16] = {0x23, 0x55, 0x3c, 0xbe, 0x96, 0x37, 0xa8, 0x9d, 0x21, 0x8a, 0xe6, 0x4d, 0xae, 0x47, 0xbf, 0x35};
  //uint8_t key[16] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};
  uint8_t opc[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  //uint8_t rand[16] = {0x5b, 0x2e, 0x1c, 0x24, 0x28, 0xc7, 0x56, 0x28, 0xe4, 0x43, 0xb0, 0xac, 0x89, 0x5d, 0x5b, 0x73};
  uint8_t rand[16] = {0x72, 0x13, 0x34, 0xee, 0x3e, 0xf1, 0xcd, 0x1b, 0x10, 0xfc, 0xe3, 0x63, 0xb4, 0x52, 0xde, 0xaa};
  uint8_t res[8], ck[16], ik[16], ak[6];
  Authentication_5gaka::f2345(opc, key, rand, res, ck, ik, ak);
  printf("res: 0x\n");
  for(int i=0; i<8; i++)
    printf("%x", res[i]);
  printf("\n");
  printf("ak: 0x");
  for(int i=0; i<6; i++)
    printf("%x", ak[i]);
  printf("\n");
  printf("ck: 0x\n");
  for(int i=0; i<16; i++)
    printf("%x", ck[i]);
  printf("\n");
  printf("ik: 0x\n");
  for(int i=0; i<16; i++)
    printf("%x", ik[i]);
  printf("\n");
  uint8_t ak2[6];
  Authentication_5gaka::f5star(opc, key, rand, ak2);
  printf("ak2: 0x");
  for(int i=0; i<6; i++)
    printf("%x ", ak2[i]);
  printf("\n");
}

void res_test(){
  uint8_t rand[16] = {0x23, 0x55, 0x3c, 0xbe, 0x96, 0x37, 0xa8, 0x9d, 0x21, 0x8a, 0xe6, 0x4d, 0xae, 0x47, 0xbf, 0x35};
  //uint8_t key[32] = {0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc, 0x46, 0x5b, 0x5c, 0xe8, 0xb1, 0x99, 0xb4, 0x9f, 0xaa, 0x5f, 0x0a, 0x2e, 0xe2, 0x38, 0xa6, 0xbc};
  uint8_t key[32] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  string snn = "5G:mnc001.mcc110.3gppnetwork.org";
  OCTET_STRING_t netName;
  OCTET_STRING_fromBuf(&netName, snn.c_str(), snn.length());
  uint8_t S[100];
  S[0] = 0x6B;
  memcpy (&S[1], netName.buf, netName.size);
  printf("size = %x", netName.size);
  S[1+netName.size] = (netName.size & 0xff00)>>8;
  S[2+netName.size] = (netName.size & 0x00ff);
  //for(int i=0; i<16; i++)
  //  S[3+netName.size+i] = rand[i];
  //S[19+netName.size] = 0x00;
  //S[20+netName.size] = 0x10;
  uint8_t output[16];
  //Authentication_5gaka::kdf(key, 32, K.buf, K.size, output, 16);
  Authentication_5gaka::kdf(key, 32, S, 3+netName.size, output, 16);
  printf("inputstring ...\n");
  //for(int i=0; i< 1; i++)
  for(int i=0; i< 3+netName.size; i++)
    printf("%c", S[i]);
  printf("\n");
  printf("key ...\n");
  for(int i=0; i< 32; i++)
    printf("%x", key[i]);
  printf("\n");
  printf("out ...\n");
  for(int i=0; i< 16; i++)
    printf("%x ", output[i]);
  printf("\n");
}

#include "nas_algorithms.hpp"

void nia1_test(){
  uint32_t count = 0x38a6f056;
  uint8_t key[16] = {0x2b, 0xd6, 0x45, 0x9f, 0x82, 0xc5, 0xb3, 0x00, 0x95, 0x2c, 0x49, 0x10, 0x48, 0x81, 0xff, 0x48};
  uint8_t message[16] = {0x33, 0x32, 0x34, 0x62, 0x63, 0x39, 0x38, 0x61, 0x37, 0x34, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key = key;
  stream_cipher.key_length = 16;
  stream_cipher.count = count;
  stream_cipher.bearer = 0x1f; 
  stream_cipher.direction = 0;
  stream_cipher.message = message;
  stream_cipher.blength = 88;
  nas_algorithms::nas_stream_encrypt_nia1 (&stream_cipher, mac);
  uint32_t mac32 = ntohl(*((uint32_t*) mac));
  printf("mac: 0x%x\n", mac32);
}

void nia2_test(){
  uint32_t count = 0x38a6f056;
  uint8_t key[16] = {0x2b, 0xd6, 0x45, 0x9f, 0x82, 0xc5, 0xb3, 0x00, 0x95, 0x2c, 0x49, 0x10, 0x48, 0x81, 0xff, 0x48};
  uint8_t message[8] = {0x33, 0x32, 0x34, 0x62, 0x63, 0x39, 0x38, 0x40};
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key = key;
  stream_cipher.key_length = 16;
  stream_cipher.count = count;
  stream_cipher.bearer = 0x18;
  stream_cipher.direction = 0;
  stream_cipher.message = message;
  stream_cipher.blength = 58; 
  nas_algorithms::nas_stream_encrypt_nia2 (&stream_cipher, mac);
  uint32_t mac32 = ntohl(*((uint32_t*) mac));
  printf("mac: 0x%x\n", mac32);
}

void buffer_test(){
  uint8_t mac[4] = {0x11, 0x22, 0x33, 0x44};
  uint8_t buffer[6] = {0};
  uint32_t mac32;
  mac32 = ntohl(*((uint32_t*)mac));
  memcpy(buffer, &mac32, 4);
  print_buffer("amf_n1", "uint8_t *mac", mac, 4);
  printf("mac32: 0x%x\n", mac32);
  print_buffer("amf_n1", "buffer", buffer, 4);
  *((uint32_t*)buffer) = htonl(mac32);
  print_buffer("amf_n1", "buffer2", buffer, 4);
}

void xresStar_test(){

  uint8_t opc[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  uint8_t rand[16] = {0x5b, 0x2e, 0x1c, 0x24, 0x28, 0xc7, 0x56, 0x28, 0xe4, 0x43, 0xb0, 0xac, 0x89, 0x5d, 0x5b, 0x73};
  uint8_t res[8], ck[16], ik[16], ak[6];
  Authentication_5gaka::f2345(opc, key, rand, res, ck, ik, ak);

  std::string snn = "5G:mnc001.mcc110.3gppnetwork.org";
  OCTET_STRING_t netName;
  OCTET_STRING_fromBuf(&netName, snn.c_str(), snn.length());
  uint8_t S[100];
  S[0] = 0x6B;
  memcpy (&S[1], netName.buf, netName.size);
  printf("snn length(0x%x)\n", netName.size);
  S[1+netName.size] = (netName.size & 0xff00)>>8;
  S[2+netName.size] = (netName.size & 0x00ff);
  for(int i=0; i<16; i++)
    S[3+netName.size+i] = rand[i];
  S[19+netName.size] = 0x00;
  S[20+netName.size] = 0x10;
  for(int i=0; i<8; i++)
    S[21+netName.size+i] = res[i];
  S[29+netName.size] = 0x00;
  S[30+netName.size] = 0x08;

  uint8_t ckik[32];
  memcpy (&key[0], ck, 16);
  memcpy (&key[16], ik, 16);//KEY

  uint8_t out[32], output[16];
  Authentication_5gaka::kdf(ckik, 32, S, 31+netName.size, out, 32);
  for(int i=0; i<16; i++)
    output[i] = out[16+i];
  print_buffer("amf_n1", "XRES*", output, 16);
}

void mac_test(){
  uint8_t opc[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
  uint8_t key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  uint8_t rand[16] = {0x6a, 0x89, 0x59, 0xfb, 0x18, 0x8c, 0x73, 0x30, 0x8d, 0x67, 0x9f, 0x7b, 0xc8, 0x31, 0x3d, 0x65};
  //uint8_t rand[16] = {0x02, 0x27, 0x40, 0x55, 0xea, 0x5a, 0x2f, 0x7e, 0x3e, 0x87, 0x16, 0x4b, 0xdf, 0x80, 0xff, 0x32};
  //uint8_t rand[16] = {0x97, 0xce, 0xf6, 0x4e, 0x75, 0xbf, 0x90, 0x7c, 0x01, 0xf0, 0x65, 0x6a, 0xbb, 0xf8, 0x84, 0x02};
  uint8_t res[8], ck[16], ik[16], ak[6];
  Authentication_5gaka::f2345(opc, key, rand, res, ck, ik, ak);
  print_buffer("amf_n1", "XRES = F2(RAND)", res, 8);
  print_buffer("amf_n1", "CK = F3(RAND)", ck, 16);
  print_buffer("amf_n1", "IK = F4(RAND)", ik, 16);
  print_buffer("amf_n1", "IK = F4(RAND)", ak, 16);
/*
  uint8_t CK[16] = {0xb1, 0xaa, 0xdd, 0xdb, 0x49, 0x84, 0x0f, 0x49, 0x65, 0x6f, 0xf2, 0x70, 0x31, 0x44, 0x97, 0x86};
  uint8_t IK[16] = {0x39, 0x8a, 0x1b, 0xc9, 0x2e, 0x35, 0xbd, 0x89, 0xb4, 0xe9, 0x5d, 0x03, 0xf4, 0x83, 0x5b, 0x37};
  memcpy(ck, CK, 16);
  memcpy(ik, IK, 16);
*/
/******************** XRES to XRES* **************************/
  //std::string snnStr = "5G:mnc002.mcc460.3gppnetwork.org";
  std::string snnStr = "5G:mnc001.mcc110.3gppnetwork.org";
  uint8_t * snn = (uint8_t*)snnStr.c_str();
  int len = snnStr.length();
  uint8_t KEY[32];
  memcpy(&KEY[0], ck, 16);
  memcpy(&KEY[16], ik, 16);
  print_buffer("amf_n1", "XRES* Derivation: KEY", KEY, 32);

  uint8_t S[200];
  S[0] = 0x6B;
  memcpy(&S[1], snn, len);
  S[1+len] = (len & 0xff00)>>8;
  S[2+len] = len & 0x00ff;
  memcpy(&S[3+len], rand, 16);
  S[19+len] = 0x00;
  S[20+len] = 0x10;
  memcpy(&S[21+len], res, 8);
  S[29+len] = 0x00;
  S[30+len] = 0x08;
  print_buffer("amf_n1", "XRES* Derivation: Input", S, 31+len);
 
  uint8_t out[32], output[16];
  Authentication_5gaka::kdf(KEY, 32, S, 31+len, out, 32);
  memcpy(output, out+16, 16);
  print_buffer("amf_n1" ,"Result of KDF: XRES*", output, 16);

/******************* MAC *******************************************/
  //uint8_t sqnak[6] = {0xe4, 0x0f, 0x91, 0xf6, 0x89, 0xf3};
  //uint8_t sqnak[6] = {0x0f, 0xa2, 0x08, 0x8b, 0x0d, 0xfa};
  //uint8_t sqnak[6] = {0xcb, 0x9f, 0x58, 0xa7, 0x8d, 0x98};
  uint8_t sqnak[6] = {0x97, 0x77, 0x9b, 0x30, 0x56, 0x86};
  uint8_t sqn[6];
  for(int i=0; i<6; i++)
    sqn[i] = sqnak[i] ^ ak[i];
  print_buffer("amf_n1", "sqn", sqn, 6);
  uint8_t amf[2] = {0x90, 0x01};
  uint8_t mac_a[8];
  Authentication_5gaka::f1(opc, key, rand, sqn, amf, mac_a);
  print_buffer("amf_n1", "mac_a", mac_a, 8);

/******************** KAUSF Derivation *****************************/
  memset(S, 0, 200);
  //print_buffer("amf_n1", "reset input S", S, 200);
  S[0] = 0x6A;
  memcpy(&S[1], snn, len);
  S[1+len] = (len & 0xff00)>>8;
  S[2+len] = len & 0x00ff;
  memcpy(&S[3+len], sqnak, 6);
  S[9+len] = 0x00;
  S[10+len] = 0x06;
  print_buffer("amf_n1", "Kausf Derivation: Input", S, 11+len);

  uint8_t kausf[32];
  Authentication_5gaka::kdf(KEY, 32, S, 11+len, kausf, 32);
  print_buffer("amf_n1" ,"Result of KDF: Kausf", kausf, 32);

/******************* Kseaf Derivation *****************************/
  memset(S, 0, 200);
  //std::string newsnnStr = "5G:mnc002.mcc460.3gppnetwork.org";
  std::string newsnnStr = "5G:mnc001.mcc110.3gppnetwork.org";
  uint8_t * newsnn = (uint8_t*)newsnnStr.c_str();
  len = newsnnStr.length();
  S[0] = 0x6C;
  memcpy(&S[1], newsnn, len);
  S[1+len] = (len & 0xff00)>>8;
  S[2+len] = len & 0x00ff;
  print_buffer("amf_n1", "Kseaf Derivation: Input", S, 3+len);

  uint8_t kseaf[32];
  Authentication_5gaka::kdf(kausf, 32, S, 3+len, kseaf, 32);
  print_buffer("amf_n1" ,"Result of KDF: Kseaf", kseaf, 32);

/******************* Kamf Derivation ******************************/
  uint8_t abba[2] = {0x00, 0x00};
  //std::string supiStr = "";
  //std::string supiStr = "imsi-460110123456789";
  //std::string supiStr = "460020100001103";
  std::string supiStr = "110010123456789";
  uint8_t *supi = (uint8_t*)supiStr.c_str();
  len = supiStr.length();
  //uint8_t supi[8] = {0x46, 0x0F, 0x11, 0x01, 0x23, 0x45, 0x67, 0x89};
  //len = 8;
  memset(S, 0, 200);
  S[0] = 0x6D;
  memcpy(&S[1], supi, len);
  S[1+len] = (len & 0xff00)>>8;
  S[2+len] = len & 0x00ff;
  memcpy(&S[3+len], abba, 2);
  S[5+len] = 0x00;
  S[6+len] = 0x02;
  print_buffer("amf_n1", "Kamf Derivation: Input", S, 7+len);
  
  uint8_t kamf[32];
  Authentication_5gaka::kdf(kseaf, 32, S, 7+len, kamf, 32);
  print_buffer("amf_n1" ,"Result of KDF: Kamf", kamf, 32);

/******************** Knas_int with NIA1 Derivation ****************/ 
  memset(S, 0, 200);
  S[0] = 0x69;
  S[1] = 0x02;
  S[2] = 0x00; 
  S[3] = 0x01; 
  S[4] = 0x01;
  S[5] = 0x00; 
  S[6] = 0x01; 
  print_buffer("amf_n1", "Kamf_int with NIA1 Derivation: Input", S, 7);
  
  uint8_t kamf_int[16];
  memset(out, 0, 32);
  Authentication_5gaka::kdf(kamf, 32, S, 7, out, 32);
  memcpy(kamf_int, out+16, 16);
  print_buffer("amf_n1" ,"Result of KDF: Kamf_int", kamf_int, 16);

/********************** NIA1 Algrithms ******************************/
  uint32_t count = 0;
  uint8_t message[13] = {0x00 ,0x7e, 0x00, 0x5d, 0x01, 0x01, 0x02, 0xf0, 0xf0, 0xe1, 0x36, 0x01, 0x02};
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key = kamf_int;
  stream_cipher.key_length = 16;
  stream_cipher.count = count;
  stream_cipher.bearer = 0x01;
  stream_cipher.direction = 1;
  stream_cipher.message = message;
  stream_cipher.blength = 13*8;
  nas_algorithms::nas_stream_encrypt_nia1 (&stream_cipher, mac);
  //uint32_t mac32 = ntohl(*((uint32_t*) mac));
  print_buffer("amf_n1", "calculated MAC", mac, 4);
}

void new_mac_test(){
  uint8_t Kausf_Input[43] = {0x6a, 0x35, 0x47, 0x3a, 0x6d, 0x6e, 0x63, 0x30, 0x30, 0x32, 0x2e, 0x6d, 0x63, 0x63, 0x34, 0x36, 0x30, 0x2e, 0x33, 0x67, 0x70, 0x70, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x6f, 0x72, 0x67, 0x00, 0x20, 0x2b, 0x9f, 0x0a, 0xf9, 0x8d, 0x89, 0x00, 0x06};
  uint8_t key[32] = {0xc4, 0xda, 0x59, 0x6a, 0xd1, 0x36, 0xe7, 0xe7, 0x45, 0x32, 0x40, 0x82, 0xdd, 0xa3, 0x30, 0x98, 0x58, 0x2b, 0x9a, 0x60, 0x5d, 0x73, 0x25, 0xf4, 0x3c, 0x2b, 0x8d, 0x5a, 0x98, 0x57, 0xc7, 0xa5};
  uint8_t Kausf[32];
  Authentication_5gaka::kdf(key, 32, Kausf_Input, 43, Kausf, 32); 
  print_buffer("amf_n1", "Kausf out", Kausf, 32);
 
  uint8_t Kseaf[32];
  uint8_t Kseaf_Input[35] = {0x6c, 0x35, 0x47, 0x3a, 0x6d, 0x6e, 0x63, 0x30, 0x30, 0x32, 0x2e, 0x6d, 0x63, 0x63, 0x34, 0x36, 0x30, 0x2e, 0x33, 0x67, 0x70, 0x70, 0x6e, 0x65, 0x74, 0x77, 0x6f, 0x72, 0x6b, 0x2e, 0x6f, 0x72, 0x67, 0x00, 0x20};
  Authentication_5gaka::kdf(Kausf, 32, Kseaf_Input, 35, Kseaf, 32); 
  print_buffer("amf_n1", "Kseaf out", Kseaf, 32);
   
  uint8_t Kamf[32];
  uint8_t Kamf_Input[22] = {0x6d, 0x34, 0x36, 0x30, 0x30, 0x32, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x30, 0x33, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x02};
  Authentication_5gaka::kdf(Kseaf, 32, Kamf_Input, 22, Kamf, 32); 
  print_buffer("amf_n1", "Kseaf out", Kamf, 32);

  uint8_t Knas_int[16], out[32];
  uint8_t Knas_int_Input[7] = {0x69, 0x02, 0x00, 0x01, 0x02, 0x00, 0x01};
  Authentication_5gaka::kdf(Kamf, 32, Knas_int_Input, 7, out, 32); 
  print_buffer("amf_n1", "Knas out", out, 32);
  memcpy(Knas_int, out+16, 16);
  print_buffer("amf_n1", "Knas_int out", Knas_int, 16);

}

void istream_test(){
  char c = '1';
}
