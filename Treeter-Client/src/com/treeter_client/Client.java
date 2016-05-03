package com.treeter_client;

import java.io.*;
import java.net.*;

public class Client
{
    private final Socket connection;
    private final SocketAddress address;

    private ClientListener listener = null;
    private Thread listenerThread = null;
    private PrintWriter outputStream;

    public interface EventListener
    {
        void action();
    }

    public interface MessageEventListener
    {
        void action(String message);
    }

    private class ClientListener implements Runnable
    {
        BufferedReader inputStream = null;
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
                connection.connect(address);
                inputStream = new BufferedReader(
                        new InputStreamReader(connection.getInputStream()));
                outputStream = new PrintWriter(connection.getOutputStream(), true);
            } catch (final IOException e)
            {
                // Jeśli coś się nie udało: generuj zdarzenie błędu połączenia i uciekaj
                if (onSocketErrorListener != null)
                    onSocketErrorListener.action();
                return;
            }
            // Jeśli połączono: generuj odpowiednie zdarzenie
            if (onConnectListener != null)
                onConnectListener.action();
            // Główna pętla nasłuchująca
            for (;;)
            {
                try
                {
                    // Czekaj i odbierz wiadomość
                    String input = inputStream.readLine();
                    // Przetwarzaj wiadomość
                    onMessageListener.action(input);
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
        String ip = addr.substring(0, addr.indexOf(':'));
        int port = Integer.parseInt(addr.substring(addr.indexOf(':') + 1));
        address = new InetSocketAddress(ip, port);
        connection = new Socket();
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

    public void send(String msg)
    {
        outputStream.println(msg);
    }
}
