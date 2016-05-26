package com.treeter_client;

import javax.swing.*;
import java.io.*;
import java.net.*;
import java.security.GeneralSecurityException;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class Client
{
    private Socket connection;

    private CryptoProvider cryptoProvider;
    private AtomicBoolean cryptoEnabled = new AtomicBoolean(false);

    private ClientListener clientListener;
    private ClientSender clientSender;

    private Thread clientListenerThread;
    private Thread clientSenderThread;

    /************************************************************************/
    /*************************** Client events ******************************/
    /************************************************************************/

    public interface EventListener
    {
        void action();
    }

    public interface MessageEventListener
    {
        void action(MessageResponse messageObj);
    }

    private EventListener onErrorListener = null;
    private EventListener onConnectListener = null;
    private MessageEventListener onMessageListener = null;

    public void onMessage(MessageEventListener listener)
    {
        onMessageListener = listener;
    }

    public void onError(EventListener listener)
    {
        onErrorListener = listener;
    }

    public void onConnect(EventListener listener)
    {
        onConnectListener = listener;
    }
    /************************************************************************/
    /*************************** ClientListener *****************************/
    /************************************************************************/

    private class ClientListener implements Runnable
    {
        private DataInputStream inputStream;

        public ClientListener(DataInputStream inputStream)
        {
            this.inputStream = inputStream;
        }

        private String readMessage() throws IOException, GeneralSecurityException
        {
            // Czekaj i odbierz wiadomość
            // Wczytaj dlugosc wiadomosci
            int msgLength = inputStream.readInt();
            byte[] buffer = new byte[msgLength];
            String message;
            // Wczytaj wiadomosc
            inputStream.readFully(buffer);
            // Zdeszyfruj, jesli szyfrowanie jest aktywne
            if (cryptoEnabled.get())
                return cryptoProvider.decryptMessage(buffer);
            else
                return new String(buffer);
        }


        @Override
        public void run()
        {
            //Główna pętla nasłuchująca
            try {
                for (; ; )
                {
                    String message = readMessage();
                    // Przetwarzaj wiadomość
                    MessageResponse messageObject = MessageResponse.deserialize(message);
                    SwingUtilities.invokeLater(() -> onMessageListener.action(messageObject));
                }
            } catch (IOException e)
            {
                SwingUtilities.invokeLater(() -> onErrorListener.action());
            } catch (Exception e) //GeneralSecurityException i ParseException i inne...
            {
                e.printStackTrace();
                SwingUtilities.invokeLater(() -> onErrorListener.action());
            }
        }
    }

    /************************************************************************/
    /**************************** ClientSender ******************************/
    /************************************************************************/

    private class ClientSender implements Runnable
    {
        private DataOutputStream outputStream;
        private LinkedBlockingQueue<MessageRequest> requests;
        private boolean availableRequest = false;

        private void sendMessage(MessageRequest msg) throws GeneralSecurityException, IOException
        {
            String message = msg.serialize();
            byte[] buffer;

            if(cryptoEnabled.get())
                buffer = cryptoProvider.encryptMessage(message);
            else
                buffer = message.getBytes();

            // dlugosc wiadomosci
            int msgLength = buffer.length;
            outputStream.writeInt(msgLength);
            // tresc wiadomosci
            outputStream.write(buffer, 0, msgLength);
        }


        public ClientSender(DataOutputStream outputStream)
        {
            this.outputStream = outputStream;
            this.requests = new LinkedBlockingQueue<MessageRequest>();
        }

        @Override
        public void run()
        {
            try {
                MessageRequest request;
                for (;;)
                {
                    request = null;
                    try
                    {
                        request = requests.take();
                    } catch(InterruptedException e) { }

                    if(request != null)
                    {
                        sendMessage(request);
                    }
                }
            } catch(IOException e)
            {
                SwingUtilities.invokeLater(() -> onErrorListener.action());
            } catch(Exception e)
            {
                e.printStackTrace();
                SwingUtilities.invokeLater(() -> onErrorListener.action());
            }
        }

        public void putMessageToSend(MessageRequest msg)
        {
            requests.offer(msg);
        }
    }

    /************************************************************************/
    /**************************** ClientOpener ******************************/
    /************************************************************************/

    private class ClientOpener implements Runnable
    {
        SocketAddress address;

        public ClientOpener(SocketAddress address)
        {
            this.address = address;
        }

        @Override
        public void run()
        {
            try
            {
                connection = new Socket();
                // Nawiązanie połączenia
                connection.connect(address);
                // Inicjalizacja strumieni
                DataInputStream inputStream = new DataInputStream(new BufferedInputStream(connection.getInputStream()));
                DataOutputStream outputStream = new DataOutputStream(connection.getOutputStream());
                // Powolanie watku listenera
                clientListener = new ClientListener(inputStream);
                clientListenerThread = new Thread();
                clientListenerThread.start();
                // Powolanie watku sendera
                clientSender = new ClientSender(outputStream);
                clientSenderThread = new Thread();
                clientSenderThread.start();
                // Powolanie zdarzenia onConnect
                SwingUtilities.invokeLater(() -> onConnectListener.action());
            } catch(IOException e)
            {
                SwingUtilities.invokeLater(() -> onErrorListener.action());
            }
        }
    }

    /************************************************************************/
    /*************************** Client methods *****************************/
    /************************************************************************/

    public Client() {
        cryptoProvider = new CryptoProvider();

    }

    public void open(String address)
    {
        String ip = address.substring(0, address.indexOf(':'));
        int port = Integer.parseInt(address.substring(address.indexOf(':') + 1));

        SocketAddress addr = new InetSocketAddress(ip, port);
    }

    public void send(MessageRequest messageRequest)
    {
        clientSender.putMessageToSend(messageRequest);
    }

    public void close()
    {
        try
        {
            if (connection != null && connection.isConnected())
                connection.close();
            if (clientListenerThread.isAlive())
                clientListenerThread.join();
            if (clientSenderThread.isAlive())
                clientSenderThread.join();
        } catch (final Exception e)
        {
            e.printStackTrace();
        } finally
        {
            clientListenerThread = null;
            clientSenderThread = null;
        }
    }

    public CryptoProvider getCryptoProvider()
    {
        return this.cryptoProvider;
    }

    public void enableCrypto()
    {
        this.cryptoEnabled.set(true);
    }
}
