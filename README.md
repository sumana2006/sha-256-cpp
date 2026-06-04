# sha-256-cpp

> A from-scratch implementation of the SHA-256 cryptographic hash function in C++.

---

## What is SHA-256?

**SHA-256** (Secure Hash Algorithm - output: 256-bit) is a cryptographic hash function developed by **NIST** (National Institute of Standards and Technology) with the support of the US Government. It belongs to the SHA-2 family and remains one of the most widely trusted and deployed hash functions in the world today.

At its core, SHA-256 is a mathematically elegant idea built on two fundamental principles:


| **Confusion** | The output should bear no obvious statistical relationship to the input — achieved through bitwise operations, rotations, and non-linear functions |
| **Diffusion** | A single changed input bit should cascade and affect roughly half of all output bits (the "avalanche effect") |

These two properties together satisfy the theoretical requirements for a secure cryptographic hash function.



## How SHA-256 Works

SHA-256 processes input data through a sequence of well-defined stages:

### 1. Pre-processing (Padding)

The input message is padded so its total length is congruent to **448 mod 512** bits:

- A single `1` bit is appended to the message
- Followed by enough `0` bits to reach 448 mod 512
- The original message length (as a 64-bit big-endian integer) is appended at the end

This ensures the padded message length is an exact multiple of **512 bits** (the block size).

### 2. Initialization

SHA-256 maintains an **8-word (256-bit) internal state**, initialized with the fractional parts of the square roots of the first 8 prime numbers:

```
H0 = 6a09e667   H1 = bb67ae85   H2 = 3c6ef372   H3 = a54ff53a
H4 = 510e527f   H5 = 9b05688c   H6 = 1f83d9ab   H7 = 5be0cd19
```

It also uses **64 round constants** (K[0..63]) derived from the cube roots of the first 64 primes.

### 3. Message Schedule

Each 512-bit block is expanded into **64 32-bit words** (`W[0..63]`):

```
W[i] = M[i]                                          for i in [0, 15]
W[i] = σ1(W[i-2]) + W[i-7] + σ0(W[i-15]) + W[i-16] for i in [16, 63]
```

Where `σ0` and `σ1` are mixing functions using right-rotations and right-shifts.

### 4. Compression (64 Rounds)

A working set of 8 variables `(a, b, c, d, e, f, g, h)` is initialized from the current hash state and then updated 64 times using:

```
T1 = h + Σ1(e) + Ch(e,f,g) + K[i] + W[i]
T2 = Σ0(a) + Maj(a,b,c)

h = g,  g = f,  f = e,  e = d + T1
d = c,  c = b,  b = a,  a = T1 + T2
```

Where:
- **Ch** (Choose): `(e AND f) XOR (NOT e AND g)`
- **Maj** (Majority): `(a AND b) XOR (a AND c) XOR (b AND c)`
- **Σ0, Σ1**: Big-sigma functions using right-rotations
- **σ0, σ1**: Small-sigma functions for the message schedule

### 5. ➕ Hash Update

After each block, the compressed values are added to the running hash state:

```
H0 += a,  H1 += b,  H2 += c,  H3 += d
H4 += e,  H5 += f,  H6 += g,  H7 += h
```

### 6. Final Digest

After all blocks are processed, the 8 hash words are concatenated to produce the final **256-bit (32-byte) digest**, typically represented as a 64-character hex string.



## Security Properties

| Property | SHA-256 |
|----------|---------|
| Output size | 256 bits |
| Block size | 512 bits |
| Rounds | 64 |
| Pre-image resistance | yes |
| Second pre-image resistance | yes |
| Collision resistance | yes |
| Currently broken? | No |



## Applications

SHA-256 is used everywhere security matters:

- **Blockchain & Cryptocurrencies** — Bitcoin uses SHA-256 for proof-of-work and address generation
- **Digital Signatures** — SSL/TLS certificates, code signing
- **Password Hashing** — as part of stronger schemes (PBKDF2, bcrypt wrapping)
- **Data Integrity** — checksums for file verification
- **HMACs** — keyed message authentication codes



## Building & Running

```bash
g++ -std=c++17 -O2 -o sha256 sha256.cpp main.cpp
./sha256 "hello world"
```


## References

- [NIST FIPS PUB 180-4](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf) — Official SHA standard
- [RFC 6234](https://datatracker.ietf.org/doc/html/rfc6234) — SHA and HMAC-SHA
- [Bitcoin Wiki — SHA-256](https://en.bitcoin.it/wiki/SHA-256)


 
<p align="center">made with bitwise operations</p>
