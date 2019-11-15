/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */
#include "tomcrypt_private.h"

/**
  @file cfb_decrypt.c
  CFB implementation, decrypt data, Tom St Denis
*/

#ifdef LTC_CFB_MODE

/**
   CFB decrypt
   @param ct      Ciphertext
   @param pt      [out] Plaintext
   @param len     Length of ciphertext (octets)
   @param cfb     CFB state
   @return CRYPT_OK if successful
*/
int cfb_decrypt(const unsigned char *ct, unsigned char *pt, unsigned long len, symmetric_CFB *cfb)
{
   int err;

   LTC_ARGCHK(pt != NULL);
   LTC_ARGCHK(ct != NULL);
   LTC_ARGCHK(cfb != NULL);

   if ((err = cipher_is_valid(cfb->ecb.cipher)) != CRYPT_OK) {
       return err;
   }

   /* is blocklen/padlen valid? */
   if (cfb->ecb.blocklen < 0 || cfb->ecb.blocklen > (int)sizeof(cfb->IV) ||
       cfb->padlen   < 0 || cfb->padlen   > (int)sizeof(cfb->pad)) {
      return CRYPT_INVALID_ARG;
   }

   while (len-- > 0) {
       if (cfb->padlen == cfb->ecb.blocklen) {
          if ((err = ecb_encrypt_block(cfb->pad, cfb->IV, &cfb->ecb)) != CRYPT_OK) {
             return err;
          }
          cfb->padlen = 0;
       }
       cfb->pad[cfb->padlen] = *ct;
       *pt = *ct ^ cfb->IV[cfb->padlen];
       ++pt;
       ++ct;
       ++(cfb->padlen);
   }
   return CRYPT_OK;
}

#endif


/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
