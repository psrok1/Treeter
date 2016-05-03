package com.treeter_client;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

public class MessageView {
    private JFrame frame;
    private JList<String> messageList;
    private JScrollPane messageScrollPane;
    private JPanel inputPanel;
    private JTextField inputField;
    private JButton sendButton;

    private Vector<String> messages;

    MessageView()
    {
        frame = new JFrame();
        frame.setSize(600, 360);
        frame.setLayout(new BorderLayout());

        messageList = new JList<String>();
        messageList.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        messageList.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
        messageList.setLayoutOrientation(JList.VERTICAL);
        messageList.setVisibleRowCount(-1);
        messageScrollPane = new JScrollPane();
        messageScrollPane.getViewport().add(messageList);
        frame.add(messageScrollPane, BorderLayout.CENTER);
        messageScrollPane.getVerticalScrollBar().addAdjustmentListener(e -> e.getAdjustable().setValue(e.getAdjustable().getMaximum()));

        messages = new Vector<String>();

        inputPanel = new JPanel(new GridBagLayout());
        frame.add(inputPanel, BorderLayout.SOUTH);
        final GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(4,16,8,4);

        inputField = new JTextField();
        c.weightx = 0.9;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 0;
        inputPanel.add(inputField, c);

        sendButton = new JButton("Wyślij");
        c.weightx = 0.1;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 1;
        c.gridy = 0;

        inputPanel.add(sendButton, c);
    }

    public void addMessage(String str)
    {
        messages.add(str);
        messageList.setListData(messages);
        messageList.revalidate();
        messageList.repaint();
    }

    public static void main(String args[])
    {
        MessageView view = new MessageView();
        view.frame.setVisible(true);
    }

    public void show()
    {
        frame.setVisible(true);
    }

    public void hide()
    {
        frame.setVisible(false);
    }


    public void attachController(MainController controller)
    {
        sendButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e) {
                controller.send(inputField.getText());
            }
        });
    }
}
