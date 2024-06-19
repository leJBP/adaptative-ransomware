# Adaptative ransomware

Adaptative encryption method ransomware.

## Description

This ransomware project is an attempt to create a ransomware which is able to choose among few encryption methods (AES, Chacha20, RSA) in aim to optimize time performance.
The ransomware is in two parts, the first one is the C2 which deal with key storage and generation. The second one is the ransomware executed on victim PC.

For now this ransomware is only useable on Linux based system due to the way we handle the benchmark.

## Legal disclaimer

Usage of anything presented in this repo to attack targets without prior mutual consent is illegal. It's the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program. Only use for educational purposes.

## Installation requirements

### C2 side

### Ransomware side

## Usage

## To Do list

### Victim side

- [x] Find files to encrypt
- [x] Retrieve key for encryption from attacker server
- [x] Retrieve key for decryption from attacker server
- [x] Encrypt RSA
- [x] Decrypt RSA
- [x] Modification RSA pour gros fichiers
- [x] Benchmark (local)
- [x] Benchmark (serveur)
- [x] Encrypt AES-256
- [x] Decrypt AES-256
- [x] Refactoring functions
- [x] Encrypt Chacha20
- [x] Decrypt Chacha20
- [x] Main file
- [x] Docker environment to test ransomware
- [ ] Daemon
- [ ] Dropper
- [ ] Multi thread encryption process

### C2 side

- [x] Key generation
- [x] Key storage in database
- [x] Benchmark scoring
- [ ] HTTPS
- [ ] Refactoring victim ID

## Inspiration

https://0x00sec.org/t/how-ransomware-works-and-gonnacry-linux-ransomware/4594

## Syntax used for C development

https://stackoverflow.com/questions/1722112/what-are-the-most-common-naming-conventions-in-c

- Trivial Variables: i,n,c,etc... (Only one letter. If one letter isn't clear, then make it a Local Variable)
- Local Variables: camelCase
- Global Variables: g_camelCase
- Const Variables: ALL_CAPS
- Pointer Variables: add a p_ to the prefix. For global variables it would be gp_var, for local variables p_var, for const variables p_VAR. If far pointers are used then use an fp_ instead of p_.
- Structs: ModulePascalCase (Module = full module name, or a 2-3 letter abbreviation, but still in PascalCase.)
- Struct Member Variables: camelCase
- Enums: ModulePascalCase
- Enum Values: ALL_CAPS
- Public Functions: ModulePascalCase
- Private Functions: PascalCase
- Macros: PascalCase

I typedef my structs, but use the same name for both the tag and the typedef. The tag is not meant to be commonly used. Instead it's preferrable to use the typedef. I also forward declare the typedef in the public module header for encapsulation and so that I can use the typedef'd name in the definition.
