package com.treeter_client;

import java.io.IOException;
import java.security.GeneralSecurityException;

public class MainController
{
    private ConnectView connectView;
    private MessageView messageView;
    private Client client;

    private class MessageProcessor implements IMessageProcessor
    {
        @Override
        public void processMessage(EchoResponse response)
        {
            messageView.addMessage(response.getMessage());
        }

        @Override
        public void processMessage(HelloResponse helloResponse)
        {
            try {
                client.getCryptoProvider().importRSAPublicKey(helloResponse.getPublicKey());
                client.getCryptoProvider().generateAESKey();
                String aesKey = client.getCryptoProvider().exportAESKey();
                // @todo
                // send StartEncryptionRequest
            } catch(IOException e) {

            } catch(GeneralSecurityException e) {

            }
        }

        @Override
        public void processMessage(StartEncryptionResponse startEncryptionResponse)
        {
            connectView.hide();
            messageView.show();
        }
    }

    private MessageProcessor messageProcessor;

    MainController()
    {
        messageProcessor = new MessageProcessor();
    }

    public void connect(String address)
    {
        client = new Client(address);
        client.onConnect(new Client.EventListener()
        {
            @Override
            public void action()
            {
                // @todo
                //client.send(new HelloRequest());
                // send HelloRequest
            }
        });
        client.onMessage(new Client.MessageEventListener()
        {
            @Override
            public void action(MessageResponse message)
            {
                message.process(messageProcessor);
            }
        });
        client.onSuddenDisconnect(new Client.EventListener()
        {
            @Override
            public void action()
            {
                connectView.show();
                messageView.hide();
            }
        });
        client.onSocketError(new Client.EventListener()
        {
            @Override
            public void action() {
                connectView.show();
                messageView.hide();
            }
        });
        client.open();
    }

    public void send(String message)
    {
        try {
            client.send(new EchoRequest(message));
        } catch(IOException e)
        {

        } catch(GeneralSecurityException e)
        {

        }
    }

    public static void main(String args[])
    {
        MainController controller = new MainController();
        controller.connectView = new ConnectView();
        controller.messageView = new MessageView();
        controller.connectView.attachController(controller);
        controller.messageView.attachController(controller);
        controller.connectView.show();
    }

    public void disconnect()
    {
        client.close();
        messageView.hide();
        connectView.show();
    }
}
