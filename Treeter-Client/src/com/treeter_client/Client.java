package com.treeter_client;

import java.io.*;
import java.net.*;
import java.security.GeneralSecurityException;

public class Client
{
    private final Socket connection;
    private final SocketAddress address;

    private CryptoProvider cryptoProvider;
    private boolean cryptoEnabled = false;
    private ClientListener listener = null;
    private Thread listenerThread = null;
    private DataOutputStream outputStream;

    public interface EventListener
    {
        void action();
    }

    public interface MessageEventListener
    {
        void action(MessageResponse messageObj);
    }

    private class ClientListener implements Runnable
    {
        DataInputStream inputStream = null;
        boolean expectedClose = false;

        public void close()
        {
            expectedClose = true;
        }

        public void run()
        {
            // Nawiązanie połączenia i inicjalizacja strumieni
            try
            {
                System.out.println("Connecting...");
                connection.connect(address);
                inputStream = new DataInputStream(
                        new BufferedInputStream(connection.getInputStream()));
                outputStream = new DataOutputStream(connection.getOutputStream());
            } catch (final IOException e)
            {
                System.out.println("ARGH! NO!");
                // Jeśli coś się nie udało: generuj zdarzenie błędu połączenia i uciekaj
                if (onSocketErrorListener != null)
                    onSocketErrorListener.action();
                return;
            }
            // Jeśli połączono: generuj odpowiednie zdarzenie
            System.out.println("Connected.");
            if (onConnectListener != null)
                onConnectListener.action();
            // Główna pętla nasłuchująca
            for (;;)
            {
                try
                {
                    // Czekaj i odbierz wiadomość
                    // wczytaj dlugosc wiadomosci
                    int msgLength = inputStream.readInt();
                    byte[] buffer = new byte[msgLength];
                    String message;
                    // wczytaj wiadomosc
                    inputStream.readFully(buffer);
                    if(cryptoEnabled)
                        message = cryptoProvider.decryptMessage(buffer);
                    else
                        message = new String(buffer);

                    if(message == null)
                    {
                        if (!expectedClose)
                        {
                            // Generuj odpowiednie zdarzenie
                            if (onSuddenDisconnectListener != null)
                                onSuddenDisconnectListener.action();
                        }
                        return;
                    }
                    System.out.println("Received");
                    // Przetwarzaj wiadomość
                    MessageResponse messageObject = MessageResponse.deserialize(message);
                    onMessageListener.action(messageObject);
                } catch (final IOException e)
                {
                    // Jeśli zamknięcie było niespodziewane
                    if (!expectedClose)
                    {
                        // Generuj odpowiednie zdarzenie
                        if (onSuddenDisconnectListener != null)
                            onSuddenDisconnectListener.action();
                    }
                    return;
                } catch (final Exception e)
                {
                    e.printStackTrace();
                    // Jeśli inny błąd...
                    try
                    {
                        // Zamknij połączenie
                        connection.close();
                    } catch (final IOException ioe)
                    {
                    }
                    // Następnie generuj odpowiednie zdarzenie
                    if (onSocketErrorListener != null)
                        onSocketErrorListener.action();
                    return;
                }
            }
        }
    }

    private EventListener onSuddenDisconnectListener = null;
    private EventListener onSocketErrorListener = null;
    private EventListener onConnectListener = null;
    private MessageEventListener onMessageListener = null;

    public Client(String addr)
    {
        cryptoProvider = new CryptoProvider();
        String ip = addr.substring(0, addr.indexOf(':'));
        int port = Integer.parseInt(addr.substring(addr.indexOf(':') + 1));
        System.out.println(String.format("%s: %d", ip, port));
        address = new InetSocketAddress(ip, port);
        connection = new Socket();
    }

    public CryptoProvider getCryptoProvider()
    {
        return cryptoProvider;
    }

    public void onMessage(MessageEventListener listener)
    {
        onMessageListener = listener;
    }

    public void onSuddenDisconnect(EventListener listener)
    {
        onSuddenDisconnectListener = listener;
    }

    public void onSocketError(EventListener listener)
    {
        onSocketErrorListener = listener;
    }

    public void onConnect(EventListener listener)
    {
        onConnectListener = listener;
    }

    public void open()
    {
        listener = new ClientListener();
        listenerThread = new Thread(listener);
        listenerThread.start();
    }

    public void close()
    {
        try
        {
            if (listener != null)
                listener.close();
            if (connection != null && connection.isConnected())
                connection.close();
            if (listenerThread.isAlive())
                listenerThread.join();
        } catch (final Exception e)
        {
            e.printStackTrace();
        } finally
        {
            listener = null;
            listenerThread = null;
        }
    }

    public void send(MessageRequest msg) throws GeneralSecurityException, IOException
    {
        String message = msg.serialize();
        byte[] buffer;

        if(cryptoEnabled)
            buffer = cryptoProvider.encryptMessage(message);
        else
            buffer = message.getBytes();

        // dlugosc wiadomosci
        int msgLength = buffer.length;
        outputStream.writeInt(msgLength);
        // tresc wiadomosci
        outputStream.write(buffer, 0, msgLength);
    }

    public void enableCrypto()
    {
        this.cryptoEnabled = true;
    }
}
