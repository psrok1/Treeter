package com.treeter_client;

import com.treeter_client.Model.GroupMessageListModel;
import com.treeter_client.Model.GroupModel;
import com.treeter_client.Model.GroupSubgroupListModel;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupSubgroupView extends JPanel
{
    JLabel panelTitle;

    JPopupMenu subgroupMenu;
    JMenuItem subgroupJoin;
    JMenuItem subgroupDelete;

    JList<String> subgroupList;
    JScrollPane subgroupScrollPane;

    private final JPanel addSubgroupPanel;
    private final JLabel addSubgroupLabel;
    private final JTextField addSubgroupField;
    private final JButton addSubgroupButton;

    public GroupSubgroupView()
    {
        this.setLayout(new BorderLayout());
        this.setBorder(new EmptyBorder(10, 10, 10, 10));
        this.setBackground(Color.WHITE);

        this.setLayout(new BorderLayout());
        this.setBorder(new EmptyBorder(10, 10, 10, 10));
        this.setBackground(Color.WHITE);
        // nazwa grupy
        panelTitle = new JLabel("Lista podgrup");
        this.add(panelTitle, BorderLayout.NORTH);

        subgroupMenu = new JPopupMenu();
        subgroupMenu.add(subgroupJoin = new JMenuItem("Poproś o dodanie do grupy"));
        subgroupMenu.add(subgroupDelete = new JMenuItem("Usuń grupę"));

        subgroupList = new JList<String>();
        subgroupList.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        subgroupList.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
        subgroupList.setLayoutOrientation(JList.VERTICAL);
        subgroupList.setVisibleRowCount(-1);
        subgroupScrollPane = new JScrollPane();
        subgroupScrollPane.getViewport().add(subgroupList);
        this.add(subgroupScrollPane, BorderLayout.CENTER);

        addSubgroupPanel = new JPanel(new BorderLayout());
        addSubgroupPanel.setBackground(Color.WHITE);
        this.add(addSubgroupPanel, BorderLayout.SOUTH);

        addSubgroupLabel = new JLabel("Dodaj nową podgrupę:");
        addSubgroupPanel.add(addSubgroupLabel, BorderLayout.NORTH);

        addSubgroupField = new JTextField();
        addSubgroupPanel.add(addSubgroupField, BorderLayout.CENTER);

        addSubgroupButton = new JButton(new ImageIcon(getClass().getResource("/add.png")));
        addSubgroupButton.setPreferredSize(new Dimension(36, 36));
        addSubgroupPanel.add(addSubgroupButton, BorderLayout.EAST);
    }

    public void updateGroup(GroupModel group)
    {
        GroupSubgroupListModel model = group.getSubgroupList();
        subgroupList.setListData(model.getData());
        subgroupList.revalidate();
        subgroupList.repaint();
    }
}