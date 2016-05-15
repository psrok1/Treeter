package com.treeter_client;

import org.bouncycastle.asn1.x509.SubjectPublicKeyInfo;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openssl.jcajce.JcaPEMKeyConverter;
import org.bouncycastle.util.encoders.Base64;
import org.bouncycastle.openssl.PEMParser;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.spec.IvParameterSpec;
import java.io.IOException;
import java.io.StringReader;
import java.security.*;
import java.util.Arrays;

public class CryptoProvider
{
    private Key aesKey;
    private PublicKey rsaKey;

    CryptoProvider()
    {
        Security.addProvider(new BouncyCastleProvider());
    }

    public void importRSAPublicKey(String pem_pub_key) throws IOException
    {
        // Odkodowanie PEM z base64
        byte[] decodedPEMPubKey = Base64.decode(pem_pub_key);
        // Parsowanie PEM
        PEMParser pemParser = new PEMParser(new StringReader(new String(decodedPEMPubKey)));
        SubjectPublicKeyInfo pemObject = (SubjectPublicKeyInfo) pemParser.readObject();
        JcaPEMKeyConverter converter = new JcaPEMKeyConverter().setProvider("BC");
        // import klucza publicznego
        rsaKey = converter.getPublicKey(pemObject);
    }

    public void generateAESKey() throws GeneralSecurityException
    {
        // Generujemy 256-bitowy klucz AES
        KeyGenerator keyGen = KeyGenerator.getInstance("AES", "BC");
        keyGen.init(128, new SecureRandom());
        aesKey = keyGen.generateKey();
    }

    public String exportAESKey() throws GeneralSecurityException
    {
        byte[] keyAESEncoded = aesKey.getEncoded();
        Cipher rsaCipher = Cipher.getInstance("RSA/NONE/PKCS1Padding", "BC");
        rsaCipher.init(Cipher.ENCRYPT_MODE, rsaKey);
        return new String(Base64.encode(rsaCipher.doFinal(keyAESEncoded)));
    }

    private IvParameterSpec getIV(Cipher cipher)
    {
        byte[] iv = new byte[cipher.getBlockSize()];
        Arrays.fill( iv, (byte) 0 );
        return new IvParameterSpec(iv);
    }

    public byte[] encryptMessage(String plaintext_message) throws GeneralSecurityException
    {
        // Szyfrujemy wiadomość kluczem AES
        // (AES_256_CBC/IV 16x{0})
        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "BC");
        cipher.init(Cipher.ENCRYPT_MODE, aesKey, getIV(cipher));
        byte[] buffer = plaintext_message.getBytes();
        return cipher.doFinal(buffer);
    }

    public String decryptMessage(byte[] encrypted_message) throws GeneralSecurityException
    {
        // Deszyfrujemy wiadomość kluczem AES
        // (AES_256_CBC/IV 16x{0})
        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "BC");
        cipher.init(Cipher.DECRYPT_MODE, aesKey, getIV(cipher));
        return new String(cipher.doFinal(encrypted_message));
    }
}