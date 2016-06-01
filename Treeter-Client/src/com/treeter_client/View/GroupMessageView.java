package com.treeter_client.View;

import com.treeter_client.MainController;
import com.treeter_client.Model.GroupMessageListModel;
import com.treeter_client.Model.GroupModel;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.util.ArrayList;
import java.util.Vector;

public class GroupMessageView extends JPanel
{
    JLabel groupName;

    JList<String> messageList;
    JScrollPane messageScrollPane;

    JTextField sendMessageField;
    JButton sendMessageButton;
    JPanel sendMessagePanel;

    public GroupMessageView()
    {
        this.setLayout(new BorderLayout());
        this.setBorder(new EmptyBorder(10,10,10,10));
        this.setBackground(Color.WHITE);

        // nazwa grupy
        groupName = new JLabel("Grupa ....");
        this.add(groupName, BorderLayout.NORTH);

        // lista wiadomosci
        messageList = new JList<String>();
        messageList.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        messageList.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
        messageList.setLayoutOrientation(JList.VERTICAL);
        messageList.setVisibleRowCount(-1);
        messageScrollPane = new JScrollPane();
        messageScrollPane.getViewport().add(messageList);
        this.add(messageScrollPane, BorderLayout.CENTER);

        // panel do wysylanie wiadomosci
        sendMessagePanel = new JPanel(new BorderLayout());
        // pole do wpisania wiadomosci
        sendMessageField = new JTextField();
        sendMessagePanel.add(sendMessageField, BorderLayout.CENTER);
        // przycisk do wysylania wiadomosci
        sendMessageButton = new JButton("Wyślij");
        sendMessagePanel.add(sendMessageButton, BorderLayout.EAST);
        this.add(sendMessagePanel, BorderLayout.SOUTH);
    }


    public void updateGroup(GroupModel group)
    {
        GroupMessageListModel model = group.getMessageList();
        final JScrollBar vscroll = messageScrollPane.getVerticalScrollBar();
        final int distanceToBottom = vscroll.getMaximum() - ( vscroll.getValue() + vscroll.getVisibleAmount() );

        messageList.setListData(model.getData());
        messageList.revalidate();
        messageList.repaint();

        SwingUtilities.invokeLater( new Runnable() {
            public void run() {
                messageScrollPane.validate();

                if( 0 == distanceToBottom ) {
                    vscroll.setValue( vscroll.getMaximum() );
                }
            }
        });
    }

    public void attachController(MainController controller)
    {
        sendMessageButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String message = sendMessageField.getText();
                controller.sendMessage(message);
                sendMessageField.setText("");
            }
        });

        sendMessageField.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                sendMessageButton.doClick();
            }
        });
    }
}
