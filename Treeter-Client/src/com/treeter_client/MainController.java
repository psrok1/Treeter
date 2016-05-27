package com.treeter_client;

import com.treeter_client.Message.*;

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
            try
            {
                client.getCryptoProvider().importRSAPublicKey(helloResponse.getPublicKey());
                client.getCryptoProvider().generateAESKey();
                String aesKey = client.getCryptoProvider().exportAESKey();

                StartEncryptionRequest startEncryptionRequest = new StartEncryptionRequest(aesKey);
                client.enableCryptoForNextResponse();
                client.send(startEncryptionRequest);
            } catch(Exception e)
            {
                e.printStackTrace();
                client.close();
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
        client = new Client();
        client.onConnect(new Client.EventListener()
        {
            @Override
            public void action()
            {
                try
                {
                    System.out.println("Action!");
                    HelloRequest helloRequest = new HelloRequest();
                    client.send(new HelloRequest());
                } catch(Exception e)
                {
                    client.close();
                }
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

        client.onError(new Client.EventListener()
        {
            @Override
            public void action()
            {
                System.out.println("Closed!");
                client.close();
                System.out.println("Closed!");
                connectView.show();
                messageView.hide();
            }
        });
        client.open(address);
    }

    public void send(String message)
    {
        client.send(new EchoRequest(message));
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
