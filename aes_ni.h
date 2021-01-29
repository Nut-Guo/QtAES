#ifndef AES_NI_H
#define AES_NI_H

#include "aes.h"
#include <wmmintrin.h>
#include <QObject>
#include <QDebug>
#define AES_128_key_exp(k, rcon) aes_128_key_expansion(k, _mm_aeskeygenassist_si128(k, rcon))

class AES_NI : public AES
{
    using Block = __m128i;
private:
    Block _key_cache[20];
    Block _state;
    void print_key() {
        for(int i = 0; i < 20; ++i) {
            char buf[16];
            _mm_storeu_si128((__m128i *) buf, _key_cache[i]);
            for(int j = 0; j < 16; ++j) {
                std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)buf[j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    static Block aes_128_key_expansion(Block key, Block keygened){
        keygened = _mm_shuffle_epi32(keygened, _MM_SHUFFLE(3,3,3,3));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        return _mm_xor_si128(key, keygened);
    }
    void aes128_load_key(string enc_key){
        if(enc_key.size() < 16)
            enc_key.append(16 - enc_key.size() % 16, 0xff);
        _key_cache[0] = _mm_loadu_si128(reinterpret_cast<const Block *>(enc_key.c_str()));
        _key_cache[1]  = AES_128_key_exp(_key_cache[0], 0x01);
        _key_cache[2]  = AES_128_key_exp(_key_cache[1], 0x02);
        _key_cache[3]  = AES_128_key_exp(_key_cache[2], 0x04);
        _key_cache[4]  = AES_128_key_exp(_key_cache[3], 0x08);
        _key_cache[5]  = AES_128_key_exp(_key_cache[4], 0x10);
        _key_cache[6]  = AES_128_key_exp(_key_cache[5], 0x20);
        _key_cache[7]  = AES_128_key_exp(_key_cache[6], 0x40);
        _key_cache[8]  = AES_128_key_exp(_key_cache[7], 0x80);
        _key_cache[9]  = AES_128_key_exp(_key_cache[8], 0x1B);
        _key_cache[10] = AES_128_key_exp(_key_cache[9], 0x36);

        // generate decryption keys in reverse order.
        // k[10] is shared by last encryption and first decryption rounds
        // k[0] is shared by first encryption round and last decryption round (and is the original user key)
        // For some implementation reasons, decryption key schedule is NOT the encryption key schedule in reverse order
        _key_cache[11] = _mm_aesimc_si128(_key_cache[9]);
        _key_cache[12] = _mm_aesimc_si128(_key_cache[8]);
        _key_cache[13] = _mm_aesimc_si128(_key_cache[7]);
        _key_cache[14] = _mm_aesimc_si128(_key_cache[6]);
        _key_cache[15] = _mm_aesimc_si128(_key_cache[5]);
        _key_cache[16] = _mm_aesimc_si128(_key_cache[4]);
        _key_cache[17] = _mm_aesimc_si128(_key_cache[3]);
        _key_cache[18] = _mm_aesimc_si128(_key_cache[2]);
        _key_cache[19] = _mm_aesimc_si128(_key_cache[1]);
//        print_key();
    }
private:
    void encsi(int round) override {
        _state = _mm_aesenc_si128(_state, _key_cache[round]);
    }
    void enclastsi() override {
        _state = _mm_aesenclast_si128(_state, _key_cache[10]);
    }
    void decsi(int round) override {
        _state = _mm_aesdec_si128(_state, _key_cache[round]);
    }
    void declastsi() override {
        _state = _mm_aesdeclast_si128(_state, _key_cache[0]);
    }
    void xorsi(int key) override {
        _state = _mm_xor_si128 (_state, _key_cache[key]);
    }
public:
    AES_NI(QObject* parent = Q_NULLPTR):AES(parent), _state{0} {}
    AES_NI(const string & key, QObject* parent = Q_NULLPTR):AES(parent), _state{0} {
        set_key(key);
    }
    ~AES_NI() override{}
    void set_state(char * block) override {
        _state = _mm_loadu_si128((const Block*) block);
    }
    string get_state() const override {
        char buf[16];
        _mm_storeu_si128((__m128i *) buf, _state);
//        char newbuf[16];
//        copy(buf, buf + 16, newbuf);
        return string(buf, 16);
    }
    void set_key(const string & key) override{
        aes128_load_key(key);
    }

};

#endif // AES_NI_H
