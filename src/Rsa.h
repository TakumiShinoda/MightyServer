#ifndef Rsa_h
#define Rsa_h

#include <Arduino.h>
#include <iostream>
#include <random>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>

class Rsa{
  private:
    unsigned long long PublicKey = 0;
    unsigned long long SecretKey = 0;
    unsigned long long UniteKey = 0;
    unsigned long long Seed1 = 0;
    unsigned long long Seed2 = 0;
    std::random_device Rand;
    bool isPrime(unsigned long long num);
    unsigned long long LCM(unsigned long long val1, unsigned long long val2);
    unsigned long long GCD(unsigned long long val1, unsigned long long val2);

  public:
    Rsa(unsigned long long _seed1, unsigned long long _seed2);
    unsigned long encryption(char data);
    uint8_t decryption(unsigned long data);
    std::vector<unsigned int> divideNumber(unsigned long long val);
    unsigned long long squmod(unsigned long long base, unsigned long long sq, unsigned long long mod);
};

#endif