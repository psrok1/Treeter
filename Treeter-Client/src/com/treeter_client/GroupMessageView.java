package com.treeter_client;

import com.treeter_client.Model.GroupMessageListModel;
import com.treeter_client.Model.GroupModel;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
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
        String[] data = {"one", "two", "three", "four"};
        messageList = new JList<String>(data);
        messageList.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        messageList.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
        messageList.setLayoutOrientation(JList.VERTICAL);
        messageList.setVisibleRowCount(-1);
        messageScrollPane = new JScrollPane();
        messageScrollPane.getViewport().add(messageList);
        this.add(messageScrollPane, BorderLayout.CENTER);
        messageScrollPane.getVerticalScrollBar().addAdjustmentListener(e -> e.getAdjustable().setValue(e.getAdjustable().getMaximum()));

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
        messageList.setListData(model.getData());
        messageList.revalidate();
        messageList.repaint();
    }

    public void attachController(MainController controller)
    {

    }
}
