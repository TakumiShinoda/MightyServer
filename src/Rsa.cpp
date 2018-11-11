#include "./Rsa.h"

Rsa::Rsa(unsigned long long _seed1, unsigned long long _seed2){
  unsigned long long e = 0;
  unsigned long long d = 0;
  unsigned long long n = 0;
  unsigned long long f = 0;
  unsigned long long cnt = 1;
  unsigned long long rand = Rand() % 1000 + 1;

  Seed1 = isPrime(_seed1) ? _seed1 : 0;
  Seed2 = isPrime(_seed2) ? _seed2 : 0;
  if(Seed1 == 0 || Seed2 == 0) std::cout << "failed: " << ((Seed1 == 0) ? "Seed1" : "Seed2") << std::endl;
  n = Seed1 * Seed2;

  if(Seed1 > 0 && Seed2 > 0){
    f = LCM(Seed1 - 1, Seed2 - 1);
  }
  while(!isPrime(rand) || GCD(f, rand) != 1) rand += 1;
  e = rand;

  while(true){
    if((e * cnt) % f == 1){
      d = cnt;
      break;
    }
    cnt += 1;
  }

  std::cout << "f: " << f << std::endl;
  std::cout << "e: " << e << std::endl;
  std::cout << "d: " << d << std::endl;
  std::cout << "n: " << n << std::endl;
  PublicKey = e;
  SecretKey = d;
  UniteKey = n;
}

unsigned long Rsa::encryption(char data){
  unsigned long encrypted = squmod((unsigned int)data, PublicKey, UniteKey);

  std::cout << "encInput: " << (unsigned int)data << std::endl;
  std::cout << encrypted << std::endl;

  return encrypted;
}

uint8_t Rsa::decryption(unsigned long data){
 unsigned long long decrypted = squmod(data, SecretKey, UniteKey);

  std::cout << "decInput: " << data << std::endl;
  std::cout << decrypted << std::endl;
  
  return decrypted;
}

bool Rsa::isPrime(unsigned long long num){
  if (num < 2) return false;
  else if (num == 2) return true;
  else if (num % 2 == 0) return false;

  double sqrtNum = sqrt(num);
  for (int i = 3; i <= sqrtNum; i += 2){
    if (num % i == 0){
      return false;
    }
  }
  return true;
}

unsigned long long Rsa::LCM(unsigned long long val1, unsigned long long val2){
  unsigned long long gcd = GCD(val1, val2);

  return (val1 * val2) / gcd;
}

unsigned long long Rsa::GCD(unsigned long long val1, unsigned long long val2){
  unsigned long long larger = 0;
  unsigned long long smaller = 0;
  unsigned long long result = 0;

  val1 < val2 ? larger = val2 : larger = val1;
  val1 < val2 ? smaller = val1 : smaller = val2;
  result = larger % smaller;
  while(result != 0){
    larger = smaller;
    smaller = result;
    result = larger % smaller;
  }

  return smaller;
}

std::vector<unsigned int> Rsa::divideNumber(unsigned long long val){
  std::vector<unsigned int> result;

  while(val > 0){
    result.push_back(val % 10);
    std::cout << val % 10 << std::endl;
    val /= 10;
  }

  std::reverse(result.begin(), result.end());
  std::cout << result[0] << std::endl;
  return result;
}

unsigned long long Rsa::squmod(unsigned long long base, unsigned long long sq, unsigned long long mod){
  unsigned long long result = 0;
  unsigned long long first = 1;
  unsigned long long second = base;

  for(int i = 1; i <= sq; i++){
    result = first * second % mod;
    first = result;
  }

  return result;
}