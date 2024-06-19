# Adaptative ransomware

Adaptative encryption method ransomware.

## Description

This ransomware project is an attempt to create a ransomware which is able to choose among few encryption methods (AES, Chacha20, RSA) in aim to optimize time performance.
The ransomware is in two parts, the first one is the C2 which deal with key storage and generation. The second one is the ransomware executed on victim PC.

For now this ransomware is only useable on Linux based system due to the way we handle the benchmark.

## Legal disclaimer

Usage of anything presented in this repo to attack targets without prior mutual consent is illegal. It's the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program. Only use for educational purposes.

## Installation requirements

I recommand you to use the provided docker compose to use this project, by following this advice you can skip this section. If you really want to use it without container feel free to setup it all by yourself, you'll find bellow what you need to install.

### C2 side

The C2 use node to provide services, a postgreSQL database is used with it to store keys.

```
https://nodejs.org/en
https://www.postgresql.org/download/
```

### Ransomware side

For the ransomware you'll only need gcc in aim to compile the ransomware.

```
https://gcc.gnu.org/
```

## Usage

You can find a docker compose file in the directory docker, by using the following command you'll be able to start the infrastructure. 

```
docker compose up
```

In this infrastucture you can find the C2 and his database, furthermore there is also an other container where you can use the ransomware in a safe way.

### C2

Once the project is started you can have access to the endpoint of the nodeJS server on this following URL:

```
http://localhost:5000/api-docs/
```

### Ransomware

Once the infrastructure is started you have to connect to the victim container. To compile the ransomware use this following command:

```
make all
```

Once the ransomware is compiled you can use it in two way. The first one is in a sandbox for test purposes and the second one is unrestricted. By default, the unrestricted one will scan all following path during the indexing process, feel free to modify it in the code:

```
/home/$USERNAME/Downloads/
/home/$USERNAME/Desktop/
/home/$USERNAME/Music/
/home/$USERNAME/Pictures/
/home/$USERNAME/Videos/
/home/$USERNAME/Documents/
```

About the sandboxed mode the indexing process only scan the path `/tmp/sandbox-ransomware/` where beforehand five files are created and fill with this text `This is a useless file`.

Warning: if you encrypt in sandboxed mode you should decrypt in sandboxed mode to be able to re-encrypt. This warning also applied if you use the unrestricted mode.

#### Encryption

To encrypt in sandboxed mode you can use the following command:

```
./ransomware --encrypt
```

For the unrestricted mode use this command:

```
./ransomware --encrypt --disableSM 286755fad04869ca523320acce0dc6a4
```
#### Decryption

To decrypt in sandboxed mode you can use the following command:

```
./ransomware --decrypt
```

For the unrestricted mode use this command:

```
./ransomware --decrypt --disableSM 286755fad04869ca523320acce0dc6a4
```

## To-do list

### Ransomware side

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
- [ ] Refactoring functions
- [x] Encrypt Chacha20
- [x] Decrypt Chacha20
- [x] Main file
- [x] Docker environment to test ransomware
- [ ] Daemon
- [ ] Dropper
- [ ] Multi thread encryption process
- [ ] Usage option

### C2 side

- [x] Key generation
- [x] Key storage in database
- [x] Benchmark scoring
- [ ] HTTPS
- [ ] Refactoring victim ID
- [ ] Adapt benchmark scoring system

## Inspiration
### Sources

https://0x00sec.org/t/how-ransomware-works-and-gonnacry-linux-ransomware/4594

### Syntax used for C development

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
