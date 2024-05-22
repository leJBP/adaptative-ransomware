## But

Automatic ransomware on encryption algorithm choice

### Choix à faire

- Langage utilisé
    - Node.js (attaquant)
    - C (victime)
- Génération des clés
- Chiffrement utilisé (RSA, AES, Chacha20)
- Façon de procéder
- Stockage des clés (postgreSQL)
- Conception du programme
- Faire benchmark chiffrement en fonction de la taille des données à chiffrer

#### Victim side to do list

source : https://0x00sec.org/t/how-ransomware-works-and-gonnacry-linux-ransomware/4594

Priorité:
- [x] Find files to encrypt
- [x] Retrieve key for encryption from attacker server
- [x] Retrieve key for decryption from attacker server
- [x] Encrypt RSA
- [x] Decrypt RSA
- [ ] Encrypt AES-256
- [ ] Decrypt AES-256
- [ ] Encrypt Chacha20
- [ ] Decrypt Chacha20
- [ ] Main file
- [ ] Docker environment to test ransomware

Pas la priorité :
- [ ] Daemon
- [ ] Dropper


##### Syntaxe pour le C

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