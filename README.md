# MightyServer
  * Challenge of Tiny Power.

## Hardware
  * ESP32
  * microSD slot

## Developping Memo
  #### About when recieve upload file from client
  1. Recieve encrypted data from client.(String -> std::vector<'unsigned long long'>)
  2. Decrypt data(std::vector<'unsigned long long'>)
  (will be fix arguments type)
  3. Convert to String of utf-8(String)
  (Now, spend a lot of memory at some case)
  (Will use 'shrink_to_fit' of std::vector for optimize.)

  #### About RSA
  * Update later...