package com.treeter_client;

public class MainController
{
    private ConnectView connectView;
    private MessageView messageView;
    private Client client;

    public void connect(String address)
    {
        client = new Client(address);
        client.onConnect(new Client.EventListener() {
            @Override
            public void action() {
                connectView.hide();
                messageView.show();
            }
        });
        client.onMessage(new Client.MessageEventListener() {
            @Override
            public void action(String message) {
                messageView.addMessage(message);
            }
        });
        client.onSuddenDisconnect(new Client.EventListener() {
            @Override
            public void action() {
                connectView.show();
                messageView.hide();
            }
        });
        client.onSocketError(new Client.EventListener() {
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
        client.send(message);
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
}
