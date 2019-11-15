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
   @file cfb_start.c
   CFB implementation, start chain, Tom St Denis
*/


#ifdef LTC_CFB_MODE

/**
   Initialize a CFB context
   @param cipher      The index of the cipher desired
   @param IV          The initialization vector
   @param key         The secret key
   @param keylen      The length of the secret key (octets)
   @param num_rounds  Number of rounds in the cipher desired (0 for default)
   @param cfb         The CFB state to initialize
   @return CRYPT_OK if successful
*/
int cfb_start(int cipher, const unsigned char *IV, const unsigned char *key,
              int keylen, int num_rounds, symmetric_CFB *cfb)
{
   int x, err;

   LTC_ARGCHK(IV != NULL);
   LTC_ARGCHK(key != NULL);
   LTC_ARGCHK(cfb != NULL);

   if ((err = cipher_is_valid(cipher)) != CRYPT_OK) {
      return err;
   }

   /* init the cipher */
   if ((err = ecb_start(cipher, key, keylen, num_rounds, &cfb->ecb)) != CRYPT_OK) {
      return err;
   }
   /* copy data */
   for (x = 0; x < cfb->ecb.blocklen; x++) {
       cfb->IV[x] = IV[x];
   }

   /* encrypt the IV */
   cfb->padlen = 0;
   return ecb_encrypt_block(cfb->IV, cfb->IV, &cfb->ecb);
}

#endif

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
