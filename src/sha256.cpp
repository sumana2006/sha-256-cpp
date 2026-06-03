#include "sha256.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

//declaring the 64 hardcoded k[i]'s in hexadecimal: 
//(fractional part of cuberoots of first 64 prime nos)

static const uint32_t K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
    0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
    0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
    0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
    0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
    0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
    0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
    0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
    0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

//defining rotate to right function:

uint32_t rotr(uint32_t x, int n) {
    return (x >> n) | (x << (32 - n));
}

//defining choice function:

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

//defining majority function:

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

//defining Big sigma0 function: (used in compression function)

uint32_t Sigma0(uint32_t x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

//defining Big sigma1 function: (used in compression function)

uint32_t Sigma1(uint32_t x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

//defining small sigma0 function: (used in scheduling words - message scheduling - diffusion)

uint32_t sigma0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

//defining small sigma1 function: (used in scheduling words - message scheduling - diffusion)

uint32_t sigma1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

//actual overall sha256 function:

string sha256(const string& input) {

    string clean_input = input;

    // Strip trailing terminal artifacts out from inside the core function safely
    while (!clean_input.empty() && (clean_input.back() == '\n' || clean_input.back() == '\r')) {
        clean_input.pop_back();
    }

    //converting input string into bytes
    vector<uint8_t> msg(clean_input.begin(), clean_input.end());
    
    //last 64 bits of 512 bit chunk - using original bit length & converting into bytes
    uint64_t bit_len = msg.size() * 8ULL;

    //appending 1 (0x80 is hexadecimal equivalent of 10000000 which is 1 converted to byte)
    msg.push_back(0x80);

    //padding such that final padded message length = 448mod512;a dding necessary zeros
    while ((msg.size() % 64) != 56)
        msg.push_back(0x00);

    for (int i = 7; i >= 0; --i)
        msg.push_back((bit_len >> (8 * i)) & 0xFF);

    //declaring original hash words before rounds start:
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;
     
    //splitting 512 bit padded message to 16 schecdule words first each of 32 bits:
    for (size_t chunk = 0; chunk < msg.size(); chunk += 64) {

        uint32_t W[64];

        for (int i = 0; i < 16; i++) {
            size_t j = chunk + i * 4;
            W[i] =
                ((uint32_t)msg[j] << 24) |
                ((uint32_t)msg[j + 1] << 16) |
                ((uint32_t)msg[j + 2] << 8) |
                ((uint32_t)msg[j + 3]);
        }
        
        //diffusion for generating 48 more schedule words:
        for (int i = 16; i < 64; i++) {
            W[i] = sigma1(W[i - 2]) +
                   W[i - 7] +
                   sigma0(W[i - 15]) +
                   W[i - 16];
        }

        //declaring working words and assigning them accordingingly (first round)

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        //each round bit mixing process:

        for (int i = 0; i < 64; i++) {

            uint32_t T1 =
                h + Sigma1(e) +
                Ch(e, f, g) +
                K[i] +
                W[i];

            uint32_t T2 =
                Sigma0(a) +
                Maj(a, b, c);

            //updating the working words (most important step)

            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        //feed forward step after 64 rounds of bit mixing and updation on 1 block

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    //output string:

    stringstream ss;

    //concatinating hash words and converting them to hexadecimal: 

    ss << hex << setfill('0')
       << setw(8) << h0
       << setw(8) << h1
       << setw(8) << h2
       << setw(8) << h3
       << setw(8) << h4
       << setw(8) << h5
       << setw(8) << h6
       << setw(8) << h7;

    return ss.str();
}

