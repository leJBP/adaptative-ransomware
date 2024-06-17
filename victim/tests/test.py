import matplotlib.pyplot as plt

tailles = [10, 100, 1000, 10000]

aes_chiffrement = [0.10, 0.24, 3.95, 28.79]
aes_dechiffrement = [0.04, 0.49, 3.53, 25.8]

chacha20_chiffrement = [0.04, 0.28, 3.33, 31.71]
chacha20_dechiffrement = [0.09, 0.28, 3.32, 30.36]

rsa4096_chiffrement = [2.51, 16.52]
rsa4096_dechiffrement = [101.88, 1002.15]

rsa2048_chiffrement = [1.36, 9.44]
rsa2048_dechiffrement = [21, 205.20]

fig, (ax1, ax2) = plt.subplots(1, 2)
fig.suptitle('Comparaison des temps de chiffrement et de déchiffrement')

# Sous-graphiques pour le chiffrement
ax1.plot(tailles, aes_chiffrement, label='AES chiffrement')
ax1.plot(tailles, chacha20_chiffrement, label='ChaCha20 chiffrement')
ax1.plot(tailles[:2], rsa2048_chiffrement, label='RSA 2048 chiffrement')
ax1.plot(tailles[:2], rsa4096_chiffrement, label='RSA 4096 chiffrement')
ax1.set_title('Chiffrement')
ax1.set_xlabel('Taille du message (en octets)')
ax1.set_ylabel('Temps (en secondes)')
ax1.legend()

ax2.plot(tailles, aes_dechiffrement, label='AES déchiffrement')
ax2.plot(tailles, chacha20_dechiffrement, label='ChaCha20 déchiffrement')
#ax2.plot(tailles[:2], rsa2048_dechiffrement, label='RSA 2048 déchiffrement')
#ax2.plot(tailles[:2], rsa4096_dechiffrement, label='RSA 4096 déchiffrement')
ax2.set_title('Déchiffrement')
ax2.set_xlabel('Taille du message (en octets)')
ax2.set_ylabel('Temps (en secondes)')
ax2.legend()

plt.show()
